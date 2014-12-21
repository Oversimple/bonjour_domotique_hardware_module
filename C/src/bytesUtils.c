#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char * stringToHex(char * string, int * nbOfBytes) {
   (*nbOfBytes) = strlen(string)/2;
   unsigned char * hexSignal = malloc((*nbOfBytes) * sizeof(unsigned char));
   int i,j=0;
   unsigned char currentByte = 0;

   for(i = 0 ; i<(*nbOfBytes) ; i++) {

      if(string[j] >= 'a') {
         currentByte = (string[j]-'a'+10) << 4;
      } else {
         currentByte = (string[j] & 0x0f) << 4;
      }
      j++;
      if(string[j] >= 'a') {
         currentByte += (string[j]-'a'+10);
      } else {
         currentByte += (string[j] & 0x0f);
      }
      j++;
      hexSignal[i] = currentByte;
   }
   return hexSignal;
}



char * hexToString(unsigned char * hex, int nbOfBytes) {
   int i;
   char * string = malloc( (nbOfBytes * 2 + 1) * sizeof(char));
   string[0] = '\0';
   char buffer[3];
   for(i=0; i < nbOfBytes; i++) {
      sprintf(buffer,"%02x", hex[i]);
      strcat(string, buffer);
   }
   return string;
}


/* Exemple 
int main(int argc, char ** argv) {

   char test[7]="ABCD14";
   int hexSize = 0;
   unsigned char * hex = stringToHex(test, &hexSize);
   int  i;

   for(i=0; i < hexSize; i++) {
      printf("%02x ",hex[i]);
   }

   printf("\n");
   printf("In string : %s",hexToString(hex, hexSize));

   return EXIT_SUCCESS;

} */
