/* 
 * File:   serial.h
 * Author: julien
 *
 * Created on December 7, 2013, 5:35 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct serial_frame {
        unsigned char header;
        unsigned short lenght;
        unsigned char *content;
    } SerialFrame;

void Serial_configSerialPort(void);
int Serial_readFrame(SerialFrame * frame);
void Serial_sendFrame(const SerialFrame * frame);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

