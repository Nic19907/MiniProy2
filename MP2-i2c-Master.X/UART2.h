/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef __UART_H
#define	__UART_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <pic16f887.h>

void uartInit (void);

//para el TX
void uart_Write (unsigned char c);

void uartWrite (unsigned char *word);

void uart_WString (char *text);

void uartEnter (void);

void uartSpace (void);

//Para recepcion

#endif	/* XC_HEADER_TEMPLATE_H */

