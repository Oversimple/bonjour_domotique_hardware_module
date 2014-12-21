#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string.h>
#include "serial_protocol.h"

int main(int argc, char **argv) {
   char c;
   serialPort p = SerialLib_open("/dev/ttyAMA0"); /* The serial port of the raspberry pi */

   printf("Come to the dark side, we have cookies :)\n");

   if(p<0) {
      printf("Serial port not open\n");
      exit(-1);
   }

   while(1) {
         write(p, "e", 1);
         tcflush(p, TCOFLUSH);
   }

   printf("serial : %d\n",p);

   SERIAL_FRAME * frame;
   SERIAL_FRAME * frameBis;

   printf("Press a key to echo\n");
   c = getchar();
   frame = newFrame(0x03,0x05, "hello");
   printf("Frame created\n");
   showFrame(frame);
   sendFrame(p,frame); 
   printf("Frame sended\n");
   frame = readFrame(p);
   printf("Frame read : ");
   showFrame(frame);

   printf("Press a key to record a trace\n");
   getchar();
   frame = newFrame(0x01,0,NULL); 
   printf("Frame created\n");
   showFrame(frame);
   sendFrame(p,frame); 
   printf("Frame sended\n");
   frame = readFrame(p);
   printf("Frame read : ");
   showFrame(frame);

   do {
      printf("Press any key to reemit the trace or q to quit\n");
      c = getchar();
      frame->header = 0x02;
      showFrame(frame);
      sendFrame(p,frame); 
      printf("Frame sended\n");
      frameBis = readFrame(p);
      printf("Frame read : ");
      showFrame(frameBis);
   } while(c != 'q');
   freeFrame(frame);
   freeFrame(frameBis);

   close(p);
   return EXIT_SUCCESS;
}
