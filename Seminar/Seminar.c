#include <xc.h>
/*======== Configurações do Código ===========*/
#pragma config OSC = HS           // Configura para cristal de alta frequência (4Mhz)
#pragma config PBADEN = OFF      // Desabilita Leitura Analogica Digital no PORTB
#pragma config WDT = OFF        // Desabilita WATCHDOG TIMER
#pragma config MCLRE = OFF     // Desabilita o pino externo de Reset

#define HIGH 0x01             // Define a palavra HIGH como 1
#define LOW  0x00            // Define a palavra LOW como 0

#define _XTAL_FREQ 4000000   // Define a frequência do cristal como 4MHz
/*================ Funções ==============*/
unsigned int map(unsigned int value, unsigned int fromLow, unsigned int fromHigh, unsigned int toLow, unsigned int toHigh);  //Função de mapeamento
void DutyCycle(void); // Função que calcula DutyCycle
/*================ Interrupção ==============*/
void __interrupt(high_priority) my_isr() {
/*======================= Leitura ADC ===================*/
    if(INTCONbits.TMR0IF) {           // Realiza as instruções caso a Flag do Timer0 esteja HIGH      

        ADCON0bits.GO = HIGH;       // Inicia a conversão
        while(ADCON0bits.GODONE);  // Espera até o fim da conversão
        unsigned int valor_adc = (ADRESH * 256) + ADRESL; // Soma os dois bytes da conversão de 10 bits e armazena na variavel
        PR2 = map(valor_adc, 0, 1023, 249, 24); // Converte o valor de 0 a 1023 para 1.000HZ a 10.000HZ
        DutyCycle();                 // Chama função para calcular DutyCycle da nova frequência
        INTCONbits.TMR0IF = LOW;    // Reseta a flag do Timer0
        TMR0 = 15536;              // Carrega o Timer0 para 50ms     
    } 
/*========================================================*/
}
int main() {
/*======================= Configuração ADC ============================*/     
    ADCON1bits.VCFG1 = LOW;            // Referência do canal ADC como VSS
    ADCON1bits.VCFG0 = LOW;           // Referência do canal ADC como VDD
    ADCON1bits.PCFG  = 0b1110;       // Define apenas AN0 como Entrada Analógica
    ADCON2bits.ADFM  = HIGH;        // Configura o Resultado justificado a Direita
    ADCON2bits.ACQT  = 0b010;      // Configura TAD para 4
    ADCON2bits.ADCS  = 0b101;     // Configura FOSC/16
    ADCON0bits.CHS   = 0b0000;   // Seleciona o canal AN0
    ADCON0bits.ADON  = HIGH;    // Habilita conversão AD
    TRISAbits.RA0    = HIGH;   // Define pino A0 como entrada
/*====================================================================*/

/*=================== Configuração do PWM =========================*/
    PR2 = 249;                    // Inicia o PR2 com Frequência de 1Khz
    DutyCycle();                 // Calcula o DutyCycle para Frequência
    CCP1CONbits.CCP1M = 0b1111; // Configura modo PWM
    TRISCbits.RC2 = LOW; // Define o pino do CCP1 como saída
    T2CONbits.T2CKPS = 0b01; // Define Prescaler do Timer2 como 4
    T2CONbits.TMR2ON = HIGH; // Habilita Timer2
/*================================================================*/

/*=================== Configuração do Timer 0 =========================*/
    INTCONbits.GIE    = HIGH;          // Habilita Interrupções Globais
    INTCONbits.PEIE   = HIGH;         // Habilita Perifericos
    INTCON2bits.TMR0IP= HIGH;         // TMR0 Alta Prioridade
    T0CONbits.T0CS    = LOW;         // Seleciona a fonte de clock do Timer0
    T0CONbits.T08BIT  = LOW;        // Configura Timer0 para 16 bits
    T0CONbits.PSA     = HIGH;      // Desabilita o Prescaler do Timer0
    RCONbits.IPEN     = HIGH;     // Habilita prioridade de interrupções
    T0CONbits.TMR0ON  = HIGH;    // Liga o Timer0 
    INTCONbits.TMR0IF = LOW;    // Zera a flag do Timer0
    INTCONbits.TMR0IE = HIGH;  // Habilita a interrupção pelo Timer0
    TMR0 = 15536;             // Carrega o Timer0 para 50ms
/*======================================================================*/
    while(HIGH);             // Loop Infinito
    return LOW;
}

void DutyCycle(void) {
    
    unsigned int DC = (50 / 100.0) * (4 * (PR2+1)); // Define o DutyCycle como 50% 
    CCPR1L = DC >> 2;                
    CCP1CONbits.DC1B = DC & 0x03;
    
}

unsigned int map(unsigned int value, unsigned int fromLow, unsigned int fromHigh, unsigned int toLow, unsigned int toHigh) {   
    return ((long)(value - fromLow) * (long)(toHigh - toLow)) / (fromHigh - fromLow) + toLow;
}
