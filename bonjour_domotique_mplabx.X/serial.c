#include "serial.h"
#include "Constants.h"
#include <pic18f24k50.h>
#include <plib/usart.h>

void Serial_configSerialPort(void) {
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH,
            16);
    //Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH ,(unsigned int)  1249);
}

int Serial_readFrame( SerialFrame * frame) {
    unsigned short i;

    if(!DataRdy1USART()) {
       return 0;
    }
       
    frame->header = Read1USART();

    if(frame->header != 0xee) {
      return 0;
    }
    disableInterupts();
    while(!DataRdy1USART())
        ;
    
    frame->header = Read1USART();

    while(!DataRdy1USART())
        ;
    frame->lenght = Read1USART();
    frame->lenght = frame->lenght << 8;
    while(!DataRdy1USART())
        ;
    frame->lenght += Read1USART();

    unsigned char* p = frame->content;
    for(i=0; i< frame->lenght; p++,i++) {
        while(!DataRdy1USART())
            ;
        (*p) = Read1USART();
    }
    return 1;
}

void Serial_sendFrame(const SerialFrame * frame) {
    unsigned short i;

    while(Busy1USART())
        ;
    putc1USART(0xee);

    while(Busy1USART())
        ;
    putc1USART(frame->header);
    while(Busy1USART())
        ;
    putc1USART(frame->lenght >> 8);
    while(Busy1USART())
            ;
    putc1USART(frame->lenght);

    unsigned char * p = frame->content;
    
    for(i=0; i < frame->lenght ; i++, p++) {
        while(Busy1USART())
            ;
        putc1USART(*p);
    }
}