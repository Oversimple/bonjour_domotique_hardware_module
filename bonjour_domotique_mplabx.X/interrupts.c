#include <pic18f24k50.h>
#include <plib/timers.h>
#include "CyclicBitBuffer.h"
#include "Constants.h"

static unsigned short count = 0;
static unsigned char oneCount = 0;
static unsigned int sendTime = 0;
static char pinValue;

void interrupt high_isr(void) {
  if(TIMER_INTERUPT_FLAG) {
     WriteTimer0(SAMPLE_TIMING);
     count++;
     
     if(CyclicBitBuffer_flags.record) {
        pinValue = PIN_433_DATA_OUT;
        if(oneCount > 5) {
            CyclicBitBuffer_write(pinValue);
            if(count == CyclicBitBuffer_sizeInBit()) {
                disableInterupts();
                CyclicBitBuffer_flags.record = 0;
                count = 0;
                oneCount = 0;
                CyclicBitBuffer_flags.recordOver = 1;
            }
       } else {
            count = 0;
            if(1 == pinValue) {
                oneCount++;
            } else {
                oneCount = 0;
            }
        }
    
     } else if(CyclicBitBuffer_flags.transmit){
          
        PIN_433_DATA_IN = CyclicBitBuffer_read();
        if(count == (unsigned short)CyclicBitBuffer_sizeInBit()) {
            sendTime++;
             if(sendTime > NUMBER_OF_SEND) {
                 disableInterupts();
                 CyclicBitBuffer_flags.transmit = 0;
                 sendTime = 0;
             }
             count = 0;
             CyclicBitBuffer_resetRead();
         }
     }
     TIMER_INTERUPT_FLAG = 0;
  }
}


void interrupt low_isr(void) {


}