/* -*- c-basic-offset: 3 -*- 
 *
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 * 
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 * #include <stdlib.h>
 * #include <stdio.h>
 * #include "serial_protocol.h"
 * 
 * 
 * int main(int argc, char **argv) {
 *    serialPort p = SerialLib_open("/dev/ttyUSB0"); 
 *    SERIAL_FRAME * frame = newFrame((unsigned char)0, (unsigned char)255,"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent neque ante, bibendum non dignissim ullamcorper, accumsan quis urna. Integer ullamcorper lectus bibendum semper aliquet. Cras pretium ligula et urna pulvinar blandit. Fusce et arcu sed amet");
 *    sendFrame(p,frame);
 *    freeFrame(frame);
 *    frame = readFrame(p);
 * 
 *    printf("recu : %s\n",frame->content);
 *    freeFrame(frame);
 * 
 *    return EXIT_SUCCESS;
 * }
 *
 */ 

/**
 * @author Julien Hatin <julien.hatin@ecole.ensicaen.fr> 
 * @version     1.0.0 - 08-12-2012
 * 
 */

#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

typedef struct serial_frame {
   unsigned char header;
   unsigned short lenght;
   unsigned char *content;
} SERIAL_FRAME;

typedef int serialPort;

typedef struct thread_values {
   int status;
   serialPort p;
   SERIAL_FRAME *frame;
} ThreadValues;


/**
 * Open the specified serial port
 * and return the associated file descriptor
 * 
 * @param serialPortPath the path to open the serial port ie : "/dev/tty0"
 * @return the fileDescriptor of the serial port
 */
serialPort SerialLib_open(const char * portPath);

/**
 *
 * Read a frame from the given serial port
 * @param fileDescriptor the fileDescriptor of the serial port
 *
 * @return the SERIAL_FRAME* read on the serialPort
 */
SERIAL_FRAME* readFrame(const serialPort fileDescriptor);


/**
 * Clear a frame from the memory
 *
 * @param frame the serial frame to erase
 * 
 */
void freeFrame(SERIAL_FRAME * frame);


/**
 * Send a frame on a serial port
 * @param fileDescriptor the fileDescriptor of the serial port
 * @param frame the frame to send
 * 
 */
void sendFrame(const serialPort fileDescriptor, const SERIAL_FRAME * frame);


/**
 * Build a frame with the given parameters
 * 
 * @param header the header of the frame
 * @param lenght the lenght of the content coded on a single unsigned char (0 to 255)
 * @param content the content of the frame
 *
 * @return A SERIAL_FRAME * configured with the given parameters
 */
SERIAL_FRAME* newFrame(const unsigned char header, const unsigned short lenght, unsigned char * content);

void * fn_process (void * args);
#endif

