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
#include "UART2.h"


/*
--------------------------------------------------------------------------------
 *                              Definiciones
--------------------------------------------------------------------------------
 */
#define _XTAL_FREQ 4000000

#define PICslave 0x50
#define SensorTemp 0x90 //1001 000?
//los datos del sensor son /2

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */

uint8_t semaforo;

struct informacion {
    uint8_t send;
    uint8_t read;
    uint8_t uart_RX;
    uint8_t map_TX;
}data;

struct sensor {
    uint8_t dec;
    uint8_t uni;
    uint8_t comparator;
}temp;

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
void config_ie      (void);

//trabajos
void colores (void);

void sens (void);

void decadas (uint8_t value, uint8_t *decadas, uint8_t *unidades);

void uart_map (void);

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        data.uart_RX = RCREG;
        PIR1bits.RCIF = 0;
    }
}
/*
--------------------------------------------------------------------------------
 *                                 Funciones
--------------------------------------------------------------------------------
 */
void colores (void) {
    //hacer que el switch cambie con el UART
    switch (data.uart_RX) {
        case '1'://verde
            data.send = 0b001;
            LCD_setCursor (2,9);
            LCD_writeString ("Verde ");
            i2c_MW(PICslave, data.send);

            //semaforo = 1;
            break;

        case '2'://amarillo
            data.send = 0b010;
            LCD_setCursor (2,9);
            LCD_writeString ("Yellow");


            i2c_MW(PICslave, data.send);

            //semaforo = 2;
            break;

        case '4'://rojo
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
}

void sens (void){
    
    
    i2c_MasterStart();
    i2c_MasterSS(0x91);
    data.read = i2c_MasterRead(0);
    data.read = data.read - 4;
    i2c_MasterStop();
    
    if (data.read != temp.comparator){
        
        uart_map();
        temp.comparator = data.read;
        
        decadas(data.read, &temp.dec, &temp.uni);
        LCD_setCursor (2,1);
        LCD_write (temp.dec + 48);       
        LCD_write (temp.uni + 48);  

        
        
        uart_Write(data.map_TX);
        //uartEnter();
        
    }
}


void decadas (uint8_t value, uint8_t *decadas, uint8_t *unidades){
    *decadas = value/10;
    *unidades = value%10;
}

void uart_map (void){
    switch (data.read) {
        case 15:
            data.map_TX = '0';
            break;
            
        case 16:
            data.map_TX = '1';
            break;
            
        case 17:
            data.map_TX = '2';
            break;
            
        case 18:
            data.map_TX = '3';
            break;
            
        case 19:
            data.map_TX = '4';
            break;
            
        case 20:
            data.map_TX = '5';
            break;
            
        case 21:
            data.map_TX = '6';
            break;
            
        case 22:
            data.map_TX = '7';
            break;
            
        case 23:
            data.map_TX = '8';
            break;
            
        case 24:
            data.map_TX = '9';
            break;
            
        case 25:
            data.map_TX = '!';
            break;
            
        case 26:
            data.map_TX = '@';
            break;
            
        case 27:
            data.map_TX = '#';
            break;
            
        case 28:
            data.map_TX = '$';
            break;
            
        case 29:
            data.map_TX = '%';
            break;
            
        case 30:
            data.map_TX = '^';
            break;
            
        case 31:
            data.map_TX = '&';
            break;
            
        case 32:
            data.map_TX = '*';
            break;
            
        case 33:
            data.map_TX = '<';
            break;
            
        case 34:
            data.map_TX = '>';
            break;
            
        default:
            data.map_TX = '+';
            break;
            
    }
}
/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */
void main(void) {
    setup();
    

    while (1){ //loop
        
        //escribir al esclavo
        colores();
        __delay_ms(200);
        
        //lectura del sensor de temperatura
        sens();      
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
    config_lcd();
    
    //steup del idc
    uartInit();
    i2c_MasterInit(400000);    //la comunicacion es de 100,000Hz
    
    temp.comparator = 0;
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

void config_ie(void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    //NO PONER TXIE, no activar la bandera del txie
    PIE1bits.RCIE = 1; //bandera de RX activada, se activa al recibir un dato
    return;
}


void config_lcd (void){
    LCD_START();
    
    LCD_setCursor (1,1);
    LCD_writeString ("Temp");
    
    LCD_setCursor (1,9);
    LCD_writeString ("Semaforo");
}

