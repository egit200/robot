/*
 *  Serial communication with Atmega328p
 *  Supports UART, Interrupts, EEPROM RW
 *  Extending to support Flash RW
 *  Datasheet: https://www.sparkfun.com/datasheets/Components/SMD/ATMega328.pdf
 *
 *  Author: Ramiz Polic
 */

// Predefined constants
// #define F_CPU 16_000_000
// #define USART_BAUDRATE 115200
#define BAUD 9600
#define UBRR_VALUE (((F_CPU / (BAUD * 16UL))) - 1)
#define RX_BUFFER_SIZE 128
#define RX_LINE_SIZE 128

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <avr/pgmspace.h>

#include <stdio.h>

#include "serial.h"
#include "portmacro.h"

// Globals
// unsigned char rx_buffer[RX_BUFFER_SIZE], rx_line[RX_LINE_SIZE];
// unsigned char rx_buffer_pos = 0, rx_line_pos = 0;
// short waiting = 1;

/************************************************************************/
/* Init module                                                          */
/************************************************************************/
inline void Serial_Init()
{
	// Initialize USART

	UBRR0H = (unsigned char)(UBRRH_VALUE >> 8);
	UBRR0L = UBRRL_VALUE;

	UCSR0B |= (1 << TXEN0);					// enable TX
											//	UCSR0B |= (1 << RXEN0);					 // enable RX
											//	UCSR0B |= (1 << RXCIE0);				 // RX complete interrupt
	UCSR0C |= (1 << USBS0) | (3 << UCSZ00); // no parity, 1 stop bit, 8-bit data
}

/************************************************************************/
/* Adding transmit modules                                              */
/************************************************************************/
inline void Serial_Char(unsigned char data)
{

	// portDISABLE_INTERRUPTS();

	while (!(UCSR0A & (1 << UDRE0)))
	{
	}

	UDR0 = data;

	// portENABLE_INTERRUPTS();
}

inline void Serial_New_Line()
{
	Serial_Char(10); // LF
					 //	Serial_Char(13); // CR
}

void Serial_String(char *s)
{
	while (*s != '\0')
		Serial_Char(*s++);
}

void Serial_String_New_Line(char *s)
{
	while (*s != '\0')
		Serial_Char(*s++);

	Serial_New_Line();
}

void Serial_Put(uint8_t number)
{
	char str[26];
    sprintf(str, "%d", number);
    Serial_String(str);
}

void Serial_Put_String_Flash(const char *cadena)
{
	for (int i = 0; i < strlen_P(cadena); i++) {
		Serial_Char((unsigned char)pgm_read_byte(&cadena[i]));
	  }
}

void Serial_Put_Long(uint16_t number) {
	char str[26];
    sprintf(str, "%d", number);
    Serial_String(str);
}
