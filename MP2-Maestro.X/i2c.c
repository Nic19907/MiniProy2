/*
 * File:   i2c.c
 * Author: NicoU
 *
 * Created on 6 de marzo de 2022, 03:14 PM
 */

#include "i2c.h"

void i2c_MasterInit (unsigned long freq){
    SSPCON2 = 0;
    //SSPADD = (_XTAL_FREQ/(4*freq))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void i2c_MasterWait (void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void i2c_MasterStart (){
    i2c_MasterWait();      //espera
    SSPCON2bits.SEN = 1;    //inicia la comunicacion i2c
}

void i2c_Master_RepeatStart (){
    i2c_MasterWait();   //espera
    SSPCON2bits.RSEN = 1;   //reinicia comunicaicon i2c
}

void i2c_MasterStop() {
    i2c_MasterWait();   //espera
    SSPCON2bits.PEN = 1;    //detiene la comunicacion i2c
}

void i2c_MasterSS(unsigned char b) {
    i2c_MasterWait();   //espera
    SSPBUF = b;
}

void i2c_MasterWrite(unsigned char c) {
    i2c_MasterWait();   //espera
    SSPBUF = c;
}

unsigned short i2c_MasterRead (unsigned short d){
    unsigned short temp;
    i2c_MasterWait();   //espera
    SSPCON2bits.RCEN = 1;
    
    i2c_MasterWait();   //espera
    temp = SSPBUF;
    
    i2c_MasterWait();   //espera
    if (d){
        SSPCON2bits.ACKDT = 0;
    }
    
    else {
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;
    return temp; //regresa el valor leido
}

void i2c_SlaveInit(unsigned char address){
    SSPADD = address;
    SSPCON = 0x36;      // 0b0011 0110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}