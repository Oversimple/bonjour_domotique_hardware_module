/* -*- c-basic-offset: 3 -*- 
 *
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Auteur1 <mail@ecole.ensicaen.fr> 
 * @author Auteur2 <mail@ecole.ensicaen.fr> 
 * @version     0.0.1 - 08-12-2009
 * 
 * @todo the list of improvements suggested for the file.
 * @bug the list of known bugs.
 */

/**
 * @file header.c
 *  
 * Description of the program objectives.

 * All necessary references.
 */

#define HEADER_SIZE 3
#define INDEX_HEADER 0
#define INDEX_LENGHT 1

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string.h>
#include "serial_protocol.h"

/* Private functions */
void safeRead(serialPort fileDescriptor, unsigned char * buffer, int lenght) {
   unsigned char *pContent;
   unsigned short i;

   for(i=0, pContent=buffer; i < lenght; i++, pContent++) {
      while(0 > read(fileDescriptor, pContent, 1)) {
      }
   }
}

void safeWrite(serialPort fileDescriptor, unsigned char * buffer, int lenght) {
   unsigned char *pContent;
   unsigned short i;

   for(i=0, pContent=buffer; i < lenght; i++, pContent++) {
         write(fileDescriptor, (const void *)pContent, 1);
         tcflush(fileDescriptor, TCOFLUSH);
         usleep(1000);
      }
   }


/* Public functions */

/**
 * Build a frame with the given parameters
 * 
 * @param header the header of the frame
 * @param lenght the lenght of the content coded on a single unsigned char (0 to 255)
 * @param content the content of the frame
 *
 * @return A SERIAL_FRAME * configured with the given parameters
 */
SERIAL_FRAME* newFrame(const unsigned char header, const unsigned short contentLenght, unsigned char * content) {
   SERIAL_FRAME * frame = malloc(sizeof(SERIAL_FRAME));
   frame->lenght = contentLenght;
   frame->header = header;
   frame->content = content;
   return frame;
}

/**
 *
 * Read a frame from the given serial port
 * @param fileDescriptor the fileDescriptor of the serial port
 *
 * @return the SERIAL_FRAME* read on the serialPort
 */
SERIAL_FRAME* readFrame(const serialPort fileDescriptor) {
   unsigned char header[HEADER_SIZE];

   safeRead(fileDescriptor, header ,1); 

   while(0xee != header[0]) {
      safeRead(fileDescriptor, header ,1); 
   }  

   SERIAL_FRAME * frame = malloc(sizeof(SERIAL_FRAME));
   safeRead(fileDescriptor, header ,HEADER_SIZE); 

   frame->header = header[INDEX_HEADER];
   frame->lenght = (header[INDEX_LENGHT] << 8) + header[INDEX_LENGHT+1];

   frame->content = malloc(frame->lenght * sizeof(unsigned char));

   safeRead(fileDescriptor, frame->content, frame->lenght);

   return frame;
}

/**
 * Clear a frame from the memory
 *
 * @param frame the serial frame to erase
 * 
 */
void freeFrame(SERIAL_FRAME * frame) {
   if(NULL != frame) {
      if(NULL !=frame->content) {
         //printf("frame->content\n");
         free(frame->content);
      }
      //printf("frame\n");
      free(frame);
   }
}

/**
 * Send a frame on a serial port
 * @param fileDescriptor the fileDescriptor of the serial port
 * @param frame the frame to send
 * 
 */
void sendFrame(const serialPort fileDescriptor, const SERIAL_FRAME * frame) {
   unsigned char *bytesFrame = malloc((frame->lenght + HEADER_SIZE + 1)*sizeof(unsigned char));
   int i;
   bytesFrame[0] = 0xee;
   bytesFrame[INDEX_HEADER + 1] = frame->header;
   bytesFrame[INDEX_LENGHT + 1] = (unsigned char)((frame->lenght & 0xff00) >> 8);
   bytesFrame[INDEX_LENGHT + 2] = (frame->lenght & 0x00ff);

   memcpy(bytesFrame + HEADER_SIZE + 1,frame->content, frame->lenght);
   safeWrite(fileDescriptor, bytesFrame ,frame->lenght + HEADER_SIZE + 1);
   free(bytesFrame);

}

/**
 * Open the specified serial port
 * and return the associated file descriptor
 * 
 * @param serialPortPath the path to open the serial port ie : "/dev/tty0"
 * @return the fileDescriptor of the serial port
 */
serialPort SerialLib_open(const char * serialPortPath) {
   int fd; /* File descriptor for the port */
   struct termios options;

   fd = open(serialPortPath, O_RDWR);
   if (fd == -1) {
      /* Error opening the serial port */
      printf("Could not open the serial port : %s - ", serialPortPath);
   }
   fcntl(fd, F_SETFL, 0);

   tcgetattr(fd, &options);

   /*
    * Set the baud rates to 115200...
    */

   cfsetispeed(&options, B57600);
   cfsetospeed(&options, B57600);

   /*
    * Enable the receiver and set local mode...
    */

   options.c_cflag |= (CLOCAL | CREAD);


   options.c_cflag &= ~CSIZE; /* Mask the character size bits */
   options.c_cflag |= CS8;    /* Select 8 data bits */

   /* No parity 8N1 */
   options.c_cflag &= ~PARENB;
   options.c_cflag &= ~CSTOPB;
   options.c_cflag &= ~CSIZE;
   options.c_cflag |= CS8;

   options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
   options.c_oflag &= ~OPOST;

   /*
    * Set the new options for the port...
    */

   tcsetattr(fd, TCSANOW, &options);


   return (serialPort)fd;
}


void showFrame(SERIAL_FRAME * frame) {
   int i;
   printf("header : %3x\n", frame->header);
   printf("lenght : %3x\n", frame->lenght);
   printf("content : ");
   for(i=0; i < frame->lenght; i++) {
      printf("%02x ", frame->content[i]);
   }
   printf("\n");
}

void * fn_process (void * args) {
   ThreadValues *threadValues = args;
   sendFrame(threadValues->p,threadValues->frame); 
   //freeFrame(threadValues->frame);
   threadValues->frame = readFrame(threadValues->p);
  // printf("set status = 0\n");
   //fflush(stdout);
   threadValues->status = 0;
   //printf("Fin fn_process\n");
   //fflush(stdout);
}
