#include "CyclicBitBuffer.h"

unsigned char writeBitIndex;
unsigned char * writeByteIndex;

unsigned char readBitIndex;
unsigned char * readByteIndex;

unsigned char CyclicBitBuffer_buffer[CyclicBitBuffer_size() ];

/* Public functions */
unsigned char * CyclicBitBuffer_getBuffer(void) {
    return CyclicBitBuffer_buffer;
}

void CyclicBitBuffer_resetRead(void) {
    readBitIndex = 1;
    readByteIndex = CyclicBitBuffer_buffer;
}

void CyclicBitBuffer_resetWrite(void) {
    writeBitIndex = 1;
    writeByteIndex = CyclicBitBuffer_buffer;
}

void CyclicBitBuffer_reset(void) {
    CyclicBitBuffer_resetRead();
    CyclicBitBuffer_resetWrite();
}

void CyclicBitBuffer_clearEvent(void) {
    CyclicBitBuffer_flags.event = 0;
    CyclicBitBuffer_flags.hasEvent = 0;
}

int CyclicBitBuffer_read() {
    int ret = (*readByteIndex) & readBitIndex;

    if(0x01 == readBitIndex) {
        readBitIndex = 0x80;
        readByteIndex++;
    } else {
        readBitIndex = readBitIndex >> 1;
    }
    
    if(0 < ret) {
        return 1;
    } else {
        return 0;
    }
}

void CyclicBitBuffer_write(int b) {
    if(b) {
        (*writeByteIndex) |= writeBitIndex;
    } else {
        (*writeByteIndex) = (*writeByteIndex) & (writeBitIndex ^ 0xff);
    }
    if(0x01 == writeBitIndex) {
        writeBitIndex = 0x80;
        writeByteIndex++;
    } else {
        writeBitIndex = writeBitIndex >> 1;
    }
}
