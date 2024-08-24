#ifndef _SERIAL_H_
#define _SERIAL_H_

extern void Serial_Init();
extern void Serial_Char(unsigned char data);
extern void Serial_Put(uint8_t number);
extern void Serial_String(char *s);
extern void Serial_String_New_Line(char *s);

#endif