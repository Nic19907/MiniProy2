/*
 * File:   i2c.c
 * Author: NicoU
 *
 * Created on 6 de marzo de 2022, 03:14 PM
 */

#include "i2c.h"

void i2c_MasterInit (unsigned long freq){
    
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    SSPSTAT = 0;
    SSPSTATbits.SMP = 0;
    
    SSPCON = 0x28; // 0b0010 1000
    
    SSPCON2 = 0;
    
    SSPADD = (_XTAL_FREQ/(4*freq))-1;
}

void i2c_MasterWait (void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)){
        //PORTA = (SSPCON2 & 0x1F) + (SSPSTATbits.R_nW<<6); //master debug
    }

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

void i2c_MasterSS(unsigned address) {
    i2c_MasterWait();   //espera
    SSPBUF = address;
}

void i2c_MasterWrite(unsigned dato) {
    i2c_MasterWait();   //espera
    SSPBUF = dato;
}




unsigned short i2c_MasterRead (unsigned short secuence){
    unsigned short temp;
    i2c_MasterWait();   //espera
    SSPCON2bits.RCEN = 1;
    
    i2c_MasterWait();   //espera
    temp = SSPBUF;
    
    i2c_MasterWait();   //espera
    if (secuence==1){
        SSPCON2bits.ACKDT = 0;
    }
    
    else {
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;
    return temp; //regresa el valor leido
}

void i2c_MW (uint8_t address, uint8_t messege){
    i2c_MasterStart();
    i2c_MasterSS(address);
    i2c_MasterWrite(messege);
    i2c_MasterStop();
}

void i2c_MR (uint8_t address, uint8_t *value){
    uint8_t temp;
    temp = address;
    temp |= 0b1; //bit0 = 1 para indicar lectura
    i2c_MasterStart();
    i2c_MasterSS(temp);
    *value = i2c_MasterRead(0);
    i2c_MasterStop();
}


void i2c_SlaveInit(uint8_t address){
    
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    SSPADD = address;
    
    SSPSTAT = 0x80;     // 0b1000 0000
    SSPSTATbits.SMP = 0;
    
    SSPCONbits.SSPEN = 1;
    SSPCONbits.CKP = 1;
    SSPCONbits.SSPM = 0b0110;
    
    SSPCON2bits.SEN = 1;
}