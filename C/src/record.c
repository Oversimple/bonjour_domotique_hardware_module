/**
 * @author Julien Hatin
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "serial_protocol.h"
#include "bytesUtils.h"
#include "frameUtils.h"

#define SEMAPHORE "/var/www/SM/semaphore"

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
   int try;
   char * string;
   long startTime;

   /* acquiring the semaphore */
   while(!access(SEMAPHORE, F_OK)) {
      usleep(50000);
   }

   FILE *fp = fopen(SEMAPHORE, "ab+");



   /* We open the pi serial port */
   threadValues.p = SerialLib_open("/dev/ttyAMA0"); /* The serial port of the raspberry pi */

   if(threadValues.p<0) {  
      fclose(fp);
      unlink(SEMAPHORE);
      return -1; 
   }

   threadValues.frame = newFrame(0x01,0,NULL); 
   threadValues.status=1;

   /* Creation of the thread for the communication */
   ret = pthread_create(&t, NULL,fn_process, &threadValues); 
   startTime = time(NULL);
   if(!ret) {
      do {
         if(0 == threadValues.status) {
            /* We extract the most relevent part of the string */
            unlink(SEMAPHORE);
            int lenght = threadValues.frame->lenght * 8;
            unsigned char * extraction = extractTrueFrame((unsigned char *)threadValues.frame->content, (int * )&lenght);

            if(NULL == extraction || 1 == lenght) {
               freeFrame(threadValues.frame);
               close(threadValues.p);
               fclose(fp);
               return -1;
            }

            string = hexToString(extraction,lenght);
            free(extraction);
            printf("%s",string);
            free(string);
            freeFrame(threadValues.frame);
            close(threadValues.p);
            fclose(fp);
            return 0;
         }while((time(NULL) - startTime) < 50);
      }
      pthread_cancel(t);
      freeFrame(threadValues.frame);
      close(threadValues.p);
      fclose(fp);
      unlink(SEMAPHORE);
      return -1;
   } 
   freeFrame(threadValues.frame);
   close(threadValues.p);
   fclose(fp);
   unlink(SEMAPHORE);
   return -1;
}

