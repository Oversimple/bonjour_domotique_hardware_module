#include <xc.h>
#include <pic18f24k50.h>
#include <plib/timers.h>
#include "Config.h"
#include "Constants.h"

void configTimer(void) {
    disableInterupts();
    TIMER_INTERUPT_FLAG = 0;
    OpenTimer0(TIMER_INT_OFF & T0_8BIT & T0_SOURCE_INT & T0_PS_1_1 & T0_EDGE_RISE);
    TIMER_INTERUPT_FLAG = 0;
}

void configPorts(void) {

    INTCON2bits.NOT_RBPU = 1;

    /* Analog input deactivation
     * Analog to digital converter setup */
    ADCON0bits.ADON = 0;
    PIE1bits.SSPIE = 0;
    CM1CON0bits.C1ON = 0;
    /* Enabling serial port*/
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    ANSELBbits.ANSB3 = 0;
    /* Debug */
    TRISBbits.TRISB4 = 0; //Enable

    /* RF433 module connnection */
    TRISBbits.TRISB0 = 1; //RECV
    TRISBbits.TRISB1 = 0; //EN
    TRISBbits.TRISB2 = 0; //TX_RX
    TRISBbits.TRISB3 = 0; //TRANSMIT
}

void configInterruptPriority(void) {
    RCONbits.IPEN = 1; //Enable hi/lo priorities

    INTCONbits.GIEH = 1; //Enable all high priority interupts
    INTCONbits.GIEL = 1; //Enble all low priotity interupts
    INTCON2bits.TMR0IP = 1; //Set timer0 high interrupt priority
}

void configClock(void) {
    OSCCONbits.IDLEN = 1; //Device enter in idle mode on sleep instruction
}
