/* 
 * File:   RF433.h
 * Author: julien
 *
 * Created on September 13, 2014, 6:18 PM
 */

#ifndef RF433_H
#define	RF433_H

#ifdef	__cplusplus
extern "C" {
#endif

    void RF433_transmit(void);
    unsigned char * RF433_receive(unsigned short * bufferLength);


#ifdef	__cplusplus
}
#endif

#endif	/* RF433_H */

