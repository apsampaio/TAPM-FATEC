/* 
 * File:   Exercicio1.c
 * Author: Andre Sampaio
 * Class: TAPM
 * 
 * Exercise: Blink 8 Leds simultaneously with a 50ms duty-cycle
 */


// Includes //

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// Pragmas //

#pragma config WDT = OFF      
#pragma config PBADEN = OFF        
#pragma config MCLRE = OFF
#pragma config PWRT = ON
#pragma config OSC = HS

// Functions //
void blinkLeds(int whichLed, int delayValue, int manyTimes);
void delay(int delayTime);
void clockLatchLed();

// Defines //

#define _XTAL_FREQ 4000000
#define Low  0x00
#define High 0x01

int main(int argc, char** argv) {
 
    while(1) {

        for (int bit = 0b00000001; bit <= 128; bit <<= 1)
            blinkLeds(bit, 50, 1);

    }

    return (EXIT_SUCCESS);
}


// Which Led will blink, Delay DutyCycle, How many times will blink
void blinkLeds(int whichLed, int delayValue, int manyTimes) {

    TRISD = Low;

    for(int i = 0; i < manyTimes; i++) {

        PORTD = whichLed;
        clockLatchLed();
        delay(delayValue);
        PORTD = Low;
        clockLatchLed();
        delay(delayValue);

    }


}

// Delay with variables
void delay(int delayTime) {

    int count = 0;
    while(count < delayTime) {
        __delay_ms(1);
        count++;
    }

}

// Low to High Clock on LED Latch
void clockLatchLed() {

    TRISEbits.TRISE0 = Low;
    PORTEbits.RE0 = Low;
    __delay_ms(1);
    PORTEbits.RE0 = High;

}
