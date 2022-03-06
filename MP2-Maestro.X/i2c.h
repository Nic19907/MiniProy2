/*
 * Permite la comunicacion 12c entre dispositivos
 * 
 * Se hizo para no tener que hacer el setup tantas veces
 * 
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __i2c_H
#define	__i2c_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

//inicializar el i2c
void i2c_MasterInit(unsigned long freq);

//funcion de espera
void i2c_MasterWait(void);

//funcion para inciar comunicacion
void i2c_MasterStart (void);

void i2c_Master_RepeatStart (void);
//funcion para seleccionar el esclavo
void i2c_MasterSS (unsigned char b);

//funcion para escribir al esclavo
void i2c_MasterWrite (unsigned char c);

//funcion para leer del esclavo
unsigned short i2c_MasterRead (unsigned short d);



//funcion para inciar el eslcavo
void i1c_SlaveInit(unsigned char address);
#endif