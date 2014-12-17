/* 
 * File:   CyclicBitBuffer.h
 * Author: julien
 *
 * Created on January 11, 2014, 6:42 PM
 */

#ifndef CYCLICBITBUFFER_H
#define	CYCLICBITBUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct cyclicbitbufferflags {
        unsigned int hasEvent : 1;
        unsigned int record: 1;
        unsigned int recordOver: 1;
        unsigned int transmit: 1;
        unsigned int event: 1;
    }CYCLICBITBUFFERFLAGS;

    CYCLICBITBUFFERFLAGS CyclicBitBuffer_flags;

#define CyclicBitBuffer_isEmpty() CyclicBitBuffer_flags.isEmpty
//#define CyclicBitBuffer_size() ((unsigned short)1024)
//#define CyclicBitBuffer_sizeInBit() ((unsigned short) 8192)

#define CyclicBitBuffer_init() CyclicBitBuffer_reset()
#define CyclicBitBuffer_size() ((unsigned short)1537)
#define CyclicBitBuffer_sizeInBit() ((unsigned short)12296) //CyclicBitBuffer_size() * 8
    
    unsigned char * CyclicBitBuffer_getBuffer(void);
    void CyclicBitBuffer_reset(void);
    void CyclicBitBuffer_resetRead(void);
    void CyclicBitBuffer_resetWrite(void);
    void CyclicBitBuffer_clearEvent(void);
    int CyclicBitBuffer_read();
    void CyclicBitBuffer_write(int b);
    //void CyclicBitBuffer_writeByte(int b);

#ifdef	__cplusplus
}
#endif

#endif	/* CYCLICBITBUFFER_H */

