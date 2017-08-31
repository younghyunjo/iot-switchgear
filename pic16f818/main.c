/* 
 * File:   newmain.c
 * Author: hyun
 *
 * Created on August 20, 2017, 2:50 PM
 */

#include <stdio.h>
#include <stdlib.h>


#define _XTAL_FREQ 31250

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB2      // CCP1 Pin Selection bit (CCP1 function on RB2)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define LED             RB5
#define LED_ON          1
#define LED_OFF         0

#define RESET           RA1
#define RESET_ACTIVE    0
#define RESET_DEACTIVE  1

#define CS              RB3
#define CS_ACTIVE       0
#define CS_DEACTIVE     1

int RCR (void)
{
    
    return 0;
}

int ResetENC28j60(void)
{
    CS = CS_DEACTIVE;
    PORTB,RB2 = 1;
    
    RESET = RESET_ACTIVE;
    __delay_ms (100);

    CS = CS_ACTIVE;
    PORTB,RB2 = 0;
    RESET = RESET_DEACTIVE;

}
int main(int argc, char** argv)
{

    SSPEN = 1;
    CKE = 1;
    
    TRISA &= 0b11111101;
    TRISB &= 0b11000011;
    ResetENC28j60();

    while(1)
    {
        LED ^= 1;
        __delay_ms(500);
    }
            
    return (EXIT_SUCCESS);
}

