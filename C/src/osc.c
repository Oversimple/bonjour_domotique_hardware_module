/**
 * @author Julien Hatin
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "serial_protocol.h"
#include "bytesUtils.h"
#include "frameUtils.h"

/**
 *
 * Ce programme enregistre une trame et appel le programe passé en argv[1] avec 
 * la reprensentation hexadecimal en ascci de la trame. En cas d'erreur le programme
 * retourne un code négatif, zero en cas de succès.
 */

int main(int argc, char ** argv) {
   pthread_t t;
   ThreadValues threadValues;
   int ret;
   char * string;
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
      while((time(NULL) - startTime) < 3) {

         if(0 == threadValues.status) {
            /* We extract the most relevent part of the string */
            int lenght = threadValues.frame->lenght ;
            string = hexToString(threadValues.frame->content,lenght);
            printf("%s",string);
            free(string);
            freeFrame(threadValues.frame);
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

