/* Macro configuration using the auto setup tool */
// PIC18F24K50 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config PLLSEL = PLL4X   // PLL Selection (4x clock multiplier)
#pragma config CFGPLLEN = ON    // PLL Enable Configuration bit (PLL Enabled)
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ = SYS48X8// Low Speed USB mode with 48 MHz system clock (System clock at 24 MHz, USB clock divider is set to 4)

// CONFIG1H
#pragma config FOSC = HSM       // Oscillator Selection (HS oscillator, medium power 4MHz to 16MHz)
#pragma config PCLKEN = OFF     // Primary Oscillator Shutdown (Primary oscillator shutdown firmware controlled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR = OFF     // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler (1:32768)

// CONFIG3H
#pragma config CCP2MX = RC1     // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config T3CMX = RC0      // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX = RC7      // SDO Output MUX bit (SDO function is on RC7)
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Block 1 Code Protect (Block 1 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protect (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)

/*
 * File:   main.c
 * Author: Julien Hatin
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f24k50.h>
#include "serial.h"
#include "Config.h"
#include "RF433.h"
#include "Constants.h"
#include "CyclicBitBuffer.h"

/*
 * 
 */
int main(int argc, char** argv) {
    unsigned short x = 0; //Blinking led counter 
    SerialFrame frame;

    /* Port and interupts configurations */
    configClock();
    configPorts();
    configInterruptPriority();
    configTimer();

    /* Initialize the serial port */
    Serial_configSerialPort();

    /* Preparing the frame */
    CyclicBitBuffer_init();

    RF433_init();
    
    /* The frame content is assigned with the cyclicBitBuffer */
    frame.content = CyclicBitBuffer_getBuffer();
    
    /* The micro initilisation is over, let's begin the main loop */
    while(1) {

        while( 0 == Serial_readFrame(&frame)) {
             x++;
             if(CyclicBitBuffer_flags.recordOver) {
                 frame.content = CyclicBitBuffer_getBuffer();
                 frame.lenght = CyclicBitBuffer_size();
                 Serial_sendFrame(&frame);
                 CyclicBitBuffer_flags.recordOver = 0;
             }

             if(0 == x) {
                PIN_DEBUG_OUTPUT ^= 1;
             }
        }

        disableInterupts();
        CyclicBitBuffer_clearEvent();
       
        /* Realise the frame management */

        if(DOM_RECORD == frame.header) {//record
            //Delay1KTCYx(255);
            frame.header = DOM_RECORD;
            RF433_receive(&(frame.lenght));
            
        } else if(DOM_TRANSMIT == frame.header) { //transmit;
            RF433_transmit();
            frame.header = DOM_TRANSMIT;
            frame.lenght = 0;
            Serial_sendFrame(&frame);

        } else if(DOM_ECHO == frame.header) {
            frame.header = DOM_ECHO_REPLY;
            Serial_sendFrame(&frame);
        } else {
            frame.header = DOM_ERROR;
            frame.lenght = 0;
            Serial_sendFrame(&frame);
        }

    }
}