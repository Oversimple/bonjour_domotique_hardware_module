/**
 * @author Julien Hatin
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include "serial_protocol.h"
#include "bytesUtils.h"
#include "frameUtils.h"

#define SEMAPHORE "/var/www/SM/semaphore"

/**
 *
 * 
 * ce programme transmet la trame passé en argv[1] 
 * argv[1] est une représentation ascii de la trame hexa à envoyer
 */

int main(int argc, char ** argv) {
   pthread_t t;
   ThreadValues threadValues;
   int ret;
   int i;
   int j;
   int try;
   unsigned char load[1537];

   /* acquiring the semaphore */
   while(!access(SEMAPHORE, F_OK)) {
      usleep(50000);
   }

   FILE *fp = fopen(SEMAPHORE, "ab+");

   printf("Call the transmit\n");

   /* We open the pi serial port */
   threadValues.p = SerialLib_open("/dev/ttyAMA0"); /* The serial port of the raspberry pi */

   if(threadValues.p<0) {
      fclose(fp);
      unlink(SEMAPHORE);
      return -1;
   }

   int lenght;
   unsigned char * content = stringToHex(argv[1],&lenght);  

   for(i=0; i < 1537; i++) {
      load[i] = content[i%lenght];
   }
   free(content);
   
   printf("Chargement effectuer\n");
   fflush(stdout);

   threadValues.frame = newFrame(0x02,1537,load); 
   threadValues.status=1;

   /* Creation of the thread for the communication */
   ret = pthread_create(&t, NULL,fn_process, &threadValues); 
   printf("Thread creer\n");
   fflush(stdout);

   if(!ret) {
   printf("Ready to loop\n");
   fflush(stdout);
      try=0;
      while(20>try) {
         usleep(250000);
         if(0 == threadValues.status) {
            printf("0 == status\n");
            fflush(stdout);
            fclose(fp);
	         unlink(SEMAPHORE);
            //freeFrame(threadValues.frame);
            printf("Freeframe\n");
            fflush(stdout);
            return 0;
         } else {
            printf(".");
            fflush(stdout);
         }
         try++;
      }
      printf("error\n");
      fflush(stdout);
      fclose(fp);
      unlink(SEMAPHORE);
      return -1;
   } 
   printf("Last error\n");
   fflush(stdout);
   freeFrame(threadValues.frame);
   fclose(fp);
   unlink(SEMAPHORE);
   return -1;
}
