/*
 * File:   main-Slave.c
 * Author: NicoU
 * Desc: Funciona como semaforo contorlado por medio de i2c
 *
 * Created on 6 de marzo de 2022, 05:49 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/*
--------------------------------------------------------------------------------
 *                              Librerias
--------------------------------------------------------------------------------
 */

//librerias de C
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>

//librerias personalizadas
#include "../MP2-Maestro.X/i2c.h"

/*
--------------------------------------------------------------------------------
 *                              Definiciones
--------------------------------------------------------------------------------
 */
#define _XTAL_FREQ 4000000
#define ADDRESS 0x50

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
struct informacion {
    uint8_t send;
    uint8_t read;
}data;

uint8_t trash =0;

/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */
//configuracion
void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_ie      (void);

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){

   if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
       
        //colision
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){ 
            SSPSTATbits.BF=0;
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
            //PORTD++;
        }

        
        //esclavo lee
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            while(!SSPSTATbits.BF);
            trash = SSPBUF;             // Lectura del SSBUF para limpiar el buffer y la bandera BF           
            //PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
        }
        
        if(SSPSTATbits.D_nA && !SSPSTATbits.R_nW){
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTA = SSPBUF;         // Guardar en el PORTD el valor del buffer de recepción
            //PORTD++;
            //PIR1bits.SSPIF = 0;    
            SSPCONbits.CKP = 1;
        }
            
        /*
        if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){ //leyendo al esclavo
            trash = SSPBUF;
            SSPSTATbits.BF = 0;
            SSPBUF = PORTB;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
         */
        PIR1bits.SSPIF = 0;    
    }
}


/*
--------------------------------------------------------------------------------
 *                                 Funciones
--------------------------------------------------------------------------------
 */

/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */

void main(void) {
    setup();
    PORTB = 0X0F;
    while (1){
        PORTB = ~PORTB;
        __delay_ms(200);
    }
    return;
}

/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */

void setup (void){
    config_io();
    config_clock();   
    config_ie();
    
    i2c_SlaveInit(ADDRESS);
}


void config_io (void) {
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    
}

void config_clock (void){
    OSCCONbits.IRCF = 0b110;    //4MHz
    OSCCONbits.SCS  = 1;    //reloj interno
}

void config_ie (void){
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    //sspov
    PIE1bits.SSPIE = 1; //INT. de i2c
    PIR1bits.SSPIF = 0; //bandera i2c
}