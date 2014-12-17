/* 
 * File:   Constants.h
 * Author: julien
 *
 * Created on January 14, 2014, 8:42 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Pin definition includes */
#include <pic18f24k50.h>

    /* Timers includes */
#include <htc.h>
#include <plib/timers.h>

    /* General constant definitions */
#define PIN_DEBUG_OUTPUT LATBbits.LATB4
//#define SAMPLE_TIMING 0xe0
#define SAMPLE_TIMING 0x50
#define NUMBER_OF_SEND 1
#define TIMER_INTERUPT_FLAG INTCONbits.TMR0IF
#define disableInterupts() (INTCONbits.TMR0IE = 0)
#define enableInterupts() (INTCONbits.TMR0IE = 1)
#define wait20uS() ( Delay100TCYx( (unsigned char) 96))
#define wait40uS() ( Delay100TCYx((unsigned char) 192 ))
#define wait200uS() ( Delay1KTCYx( (unsigned char) 96))
#define wait400uS() ( Delay1KTCYx( (unsigned char) 192))

    /* Serial */
#define PIN_SERIAL_RX PORTCbits.RX

    /* Communication with the RF module */
    
#define PIN_433_DATA_IN LATBbits.LATB3
#define PIN_433_ENABLE_OUTPUT LATBbits.LATB1
#define PIN_433_TX_RX_OUTPUT LATBbits.LATB2
#define PIN_433_DATA_OUT PORTBbits.RB0

    /* Protocol Rpi <-> pic18 */
#define DOM_RECORD 0x01
#define DOM_TRANSMIT 0x02
#define DOM_ECHO 0x03
#define DOM_ECHO_REPLY 0x04
#define DOM_START_UP 0x05
#define DOM_ERROR 0xf0

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

