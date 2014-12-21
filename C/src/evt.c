#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "serial_protocol.h"
#include "frameUtils.h"
#include "bytesUtils.h"

int record(char ** buffer) {
   pthread_t t;
   ThreadValues threadValues;
   int ret;
   long startTime;

   /* We open the pi serial port */
   threadValues.p = SerialLib_open("/dev/ttyAMA0"); /* The serial port of the raspberry pi */

   if(threadValues.p<0) {  
      return -1; 
   }

   threadValues.frame = newFrame(0x01,0,NULL); 
   threadValues.status=1;

   /* Creation of the thread for the communication */
   ret = pthread_create(&t, NULL,fn_process, &threadValues); 
   startTime = time(NULL);
   if(!ret) {
      while(1) {

         if(0 == threadValues.status) {
            /* We extract the most relevent part of the string */
            int lenght = threadValues.frame->lenght * 8;
            unsigned char * extraction = extract((unsigned char *)threadValues.frame->content, (int * )&lenght);

            if(NULL == extraction || 1 == lenght) {
               freeFrame(threadValues.frame);
               close(threadValues.p);
               return -1;
            }

            *buffer = hexToString(extraction,lenght);
            free(extraction);
            printf("'%s'\n",*buffer);
            fflush(stdout);
            //freeFrame(threadValues.frame);
            close(threadValues.p);
            return 0;
         }
      }
      pthread_cancel(t);
      freeFrame(threadValues.frame);
      close(threadValues.p);
      return -1;
   } 
   freeFrame(threadValues.frame);
   close(threadValues.p);
   return -1;
}

int main(int argc, char ** argv) {
   char *string;
   char url[200];
   int status = 1;
   url[0] = '\0';
   strcat(url, "wget \0");

   while(0 != status) {
      status = record(&string);
      if(0 == status) { 
         strcat(url,argv[1]);
         strcat(url, " --post-data=);
         strcat(url,string);
         printf("'%s'\n",url);
         fflush(stdout);
         system(url);
      }
   }

   return EXIT_SUCCESS;
}
