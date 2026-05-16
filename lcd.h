#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_clear(void);
void lcd_goto(unsigned char row, unsigned char col);
void lcd_print(char *str);

#endif
