#include "lcd.h"

void lcd_cmd(unsigned char cmd) {
    PORTA = cmd;
    PORTB &= ~(1 << PB0);
    PORTB |= (1 << PB1);
    _delay_ms(2);
    PORTB &= ~(1 << PB1);
    _delay_ms(2);
}

void lcd_data(unsigned char data) {
    PORTA = data;
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB1);
    _delay_ms(2);
    PORTB &= ~(1 << PB1);
    _delay_ms(2);
}

void lcd_init(void) {
    DDRA = 0xFF;
    DDRB |= (1 << PB0) | (1 << PB1);
    _delay_ms(20);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    _delay_ms(2);
}

void lcd_clear(void) {
    lcd_cmd(0x01);
    _delay_ms(2);
}

void lcd_goto(unsigned char row, unsigned char col) {
    if (row == 0)
        lcd_cmd(0x80 + col);
    else
        lcd_cmd(0xC0 + col);
}

void lcd_print(char *str) {
    while (*str)
        lcd_data(*str++);
}
