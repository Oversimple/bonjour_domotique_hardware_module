#include <stdio.h>
#include <stdlib.h>
#include "bytesUtils.h"
#include "frameUtils.h"


#define SEUIL 20

unsigned char * extract(unsigned char * frame, int *frameLenght) {
   int nbOfBits = (*frameLenght);
   int beginIndex = 0;
   int endIndex = 0;
   int currentZeroSuite = 0;
   int maxZeroSuite = 0;
   int i;

   for(i=0; i < nbOfBits;i++) {
      if( 0 == (frame[i/8] & (0x01 << (i%8) )) ) {
         currentZeroSuite++;
      } else {
         if(currentZeroSuite > maxZeroSuite) {
            beginIndex = i;
            maxZeroSuite = currentZeroSuite;
         }
         currentZeroSuite = 0;
      }
   }
   currentZeroSuite = 0;

   for(i=beginIndex; i < nbOfBits; i++) {
      if( 0 == (frame[i/8] & (0x01 << (i%8) )) ) {
         currentZeroSuite++;
      } else {
         if( (currentZeroSuite + SEUIL) > maxZeroSuite) {
            endIndex = i-1;
            maxZeroSuite = currentZeroSuite;
            break;
         }
         currentZeroSuite = 0;
      }
   }

   if(endIndex - beginIndex < 0) {
      return NULL;
   }
   (*frameLenght) = ( (endIndex - beginIndex)/8 ) + 1;
   unsigned char* extraction = calloc(  (*frameLenght), sizeof(unsigned char));
   int j=0;
   for(i=beginIndex; i < endIndex; i++) {
      extraction[(i-beginIndex)/8] ^= (frame[i/8] & (0x01 << (i%8)));           
   }

   return extraction;
}

int computeDistance(unsigned char * frame1, unsigned char * frame2, int lenght) {
   int i;
   int distance  = 0;
   for(i = 0; i<lenght; i++) {
      if( (frame1[i/8] & (0x01 << (i%8) )) !=  (frame2[i/8] & (0x01 << (i%8) )) ) {
         distance++;
      }
   }
   return distance;
}

unsigned char * extractTrueFrame(unsigned char * frame, int *frameLenght) {
   int nbOfBits = (*frameLenght);
   int beginIndex = 0;
   int endIndex = 0;
   int currentZeroSuite = 0;
   int maxZeroSuite = 0;
   int i;

   for(i=5000; i < nbOfBits - 3500 ;i++) {
      if( 0 == (frame[i/8] & (0x01 << (i%8) )) ) {
         currentZeroSuite++;
      } else {
         if(currentZeroSuite >= maxZeroSuite) {
            beginIndex = i;
            maxZeroSuite = currentZeroSuite;
         }
         currentZeroSuite = 0;
      }
   }
   currentZeroSuite = 0;

   for(i=beginIndex; i < nbOfBits; i++) {
      if( 0 == (frame[i/8] & (0x01 << (i%8) )) ) {
         currentZeroSuite++;
      } else {
         if( (currentZeroSuite + SEUIL) > maxZeroSuite) {
            endIndex = i-1;
            maxZeroSuite = currentZeroSuite;
            break;
         }
         currentZeroSuite = 0;
      }
   }

   //printf("beginIndex : %d/%d\n",beginIndex, (*frameLenght));
   if(endIndex - beginIndex < 0) {
      //printf("endIndex <= beginIndex\n");
      return NULL;
   }

   (*frameLenght) = ( (endIndex - beginIndex)/8 ) + 1;
   unsigned char* extraction = calloc(  (*frameLenght), sizeof(unsigned char));
   for(i=beginIndex; i < endIndex; i++) {
      extraction[(i-beginIndex)/8] ^= (frame[i/8] & (0x01 << (i%8)));           
   }
   unsigned char * subExtract;
   int lenghtSub = beginIndex;
   subExtract = extract(frame,&lenghtSub);

   if(NULL == subExtract) {
      //printf("No sub extract\n");
      unsigned char* secondChance = calloc( ((nbOfBits - endIndex) / 8) + 1 , sizeof(unsigned char));
      for(i=endIndex; i < nbOfBits; i++) {
         extraction[(i-endIndex)/8] ^= (frame[i/8] & (0x01 << (i%8)));           
      }
      int lenghtSecondChance = (nbOfBits - endIndex);
      subExtract = extract(frame,&lenghtSecondChance);
      lenghtSub = lenghtSecondChance;
      if(NULL == subExtract) {
         //printf("Second chance failed\n");
         return NULL;
      }
   }

   //printf("sub  extract : %s\n", hexToString(subExtract, lenghtSub));
   //printf("real extract : %s\n\n\n", hexToString(extraction, (*frameLenght)));

   int distanceLenght;
   int supplement;
   if(lenghtSub > (*frameLenght)) {
      distanceLenght=(*frameLenght)*8;
      supplement = (lenghtSub - (*frameLenght))*8;
   } else {
      distanceLenght=lenghtSub*8;
      supplement = ((*frameLenght) - lenghtSub)*8;
   }
   int distance = computeDistance(subExtract, extraction, distanceLenght) + supplement;

   //printf("Computed distance = %d\n\n",distance);

   free(subExtract);

   if(500 < distance) {
      return NULL;
   } else {
      return extraction;
   }

}
