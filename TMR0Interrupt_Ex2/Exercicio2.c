/* 
 * File:   Exercicio2.c
 * Author: Andre Sampaio
 * Class: TAPM
 * 
 * Exercise: TMR0 Interrupt
 */

// Includes //

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// Defines //

#define _XTAL_FREQ 4000000
#define LOW  0x00
#define HIGH 0x01

// Pragmas //

#pragma config WDT = OFF      
#pragma config PBADEN = OFF        
#pragma config MCLRE = OFF
#pragma config PWRT = ON
#pragma config OSC = HS

// Functions //

void __interrupt(high_priority) high_interrupt(void) {
    
    if(INTCONbits.TMR0IF) {
        T0CONbits.TMR0ON = LOW;
        PORTDbits.RD0 = !PORTDbits.RD0;
        INTCONbits.TMR0IF = LOW;
        TMR0 = 3035;
        T0CONbits.TMR0ON = HIGH;
    }
}

void __interrupt(low_priority) low_interrupt(void) {
        
}


int main(int argc, char** argv) {
 
    TRISDbits.RD0 = LOW;
    INTCONbits.GIE = HIGH;      //  Global Interrupt Enable bit
    INTCONbits.PEIE = HIGH;     //  Peripheral Interrupt Enable bit
    T0CONbits.T0CS = LOW;       //  Timer0 Clock Source Select bit 
    T0CONbits.T08BIT = LOW;     //  Timer0 8-Bit/16-Bit Control bit 
    T0CONbits.PSA = LOW;        //  Timer0 Prescaler Assignment bit
    T0CONbits.T0PS = 0b010;     //  1:8 Prescale value
    RCONbits.IPEN = HIGH;       //  Interrupt Priority Enable bit
    T0CONbits.TMR0ON = HIGH;    //  Timer0 On/Off Control bit     
    INTCONbits.TMR0IF = LOW;    //  TMR0 Overflow Interrupt Flag bit 
    INTCONbits.TMR0IE = HIGH;   //  TMR0 Overflow Interrupt Enable bit 
    TMR0 = 3035;                //  500ms
    
    while(1);
    
    return (EXIT_SUCCESS);
}
