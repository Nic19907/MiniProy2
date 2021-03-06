/*
 * File:   main.c
 * Author: NicoU
 * Desc: Comunicacion i2c con un esclabo y sensor de gestos y comunicacion
 * UART con un ESP32
 * Created on 6 de marzo de 2022, 02:51 PM
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
#include "lcd_8bitsA.h"
#include "i2c.h"


/*
--------------------------------------------------------------------------------
 *                              Definiciones
--------------------------------------------------------------------------------
 */
#define _XTAL_FREQ 4000000

#define PICslave 0x50

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */

uint8_t semaforo;

struct informacion {
    uint8_t send;
    uint8_t read;
}data;

/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */
//configuracion
void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_lcd     (void);

//trabajos


/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */

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
    
    
    while (1){ //loop
        switch (semaforo) {
            case 0://verde
                data.send = 0b001;
                LCD_setCursor (2,9);
                LCD_writeString ("Verde ");
                i2c_MW(PICslave, data.send);
                
                semaforo = 1;
                break;
                
            case 1://amarillo
                data.send = 0b010;
                LCD_setCursor (2,9);
                LCD_writeString ("Yellow");
                
                
                i2c_MasterStart();
                i2c_MasterSS(PICslave);
                i2c_MasterWrite(data.send);
                i2c_MasterStop();
                
                semaforo = 2;
                break;
                
            case 2://rojo
                data.send = 0b100;
                LCD_setCursor (2,9);
                LCD_writeString ("Rojo   ");
                
                i2c_MW(PICslave, data.send);
                
                semaforo = 0;
                break;
                     
            default://verde
                data.send = 0b111;
                LCD_setCursor (2,9);
                LCD_writeString ("Error ");
                
                i2c_MW(PICslave, data.send);
                
                semaforo = 1;
                break;
         
        }
    

        __delay_ms(200);

        
        
        //leer solo para el sensor
        //i2c_MR(PICslave, &PORTA);
        //__delay_ms(1000);
        
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
    config_lcd();
    
    //steup del idc
    i2c_MasterInit(400000);    //la comunicacion es de 100,000Hz
    return;
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


void config_lcd (void){
    LCD_START();
    
    LCD_setCursor (1,1);
    LCD_writeString ("Gesto");
    
    LCD_setCursor (1,9);
    LCD_writeString ("Semaforo");
}
