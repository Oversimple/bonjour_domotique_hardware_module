#include "Constants.h"
#include "CyclicBitBuffer.h"
#include "RF433.h"

void RF433_init(void) {
    PIN_433_DATA_IN = 0;

    PIN_433_ENABLE_OUTPUT = 1;
    wait20uS();
    PIN_433_TX_RX_OUTPUT = 1;
    wait200uS();
    PIN_433_TX_RX_OUTPUT = 0;
    wait40uS();
    PIN_433_ENABLE_OUTPUT = 0;
    wait200uS();
    PIN_433_ENABLE_OUTPUT = 1;
    wait200uS();
    /* ready to receive */
}

void RF433_transmit(void) {
    /* Configuring the tranceiver from RX to TX */
    PIN_433_TX_RX_OUTPUT = 1;
    wait400uS();

    CyclicBitBuffer_resetRead();
    CyclicBitBuffer_flags.transmit = 1;
    PIN_433_DATA_IN = 0;
    enableInterupts();

    while(CyclicBitBuffer_flags.transmit)
        ;

    PIN_433_DATA_IN = 0;

    /* Configuring the tranceiver from TX to RX */
    PIN_433_TX_RX_OUTPUT = 0;
    wait40uS();
    PIN_433_ENABLE_OUTPUT = 0;
    wait40uS();
    PIN_433_ENABLE_OUTPUT = 1;
    wait200uS();
}

unsigned char * RF433_receive(unsigned short * bufferLength) {
    //The transeiver is already in RX mode
    CyclicBitBuffer_reset();
    CyclicBitBuffer_flags.record = 1;
    PIN_433_DATA_IN = 0;
    enableInterupts();

    /*
    while(CyclicBitBuffer_flags.record)
        ;

    (*bufferLength) = CyclicBitBuffer_size();
    return CyclicBitBuffer_getBuffer();*/
    return 0;
}
