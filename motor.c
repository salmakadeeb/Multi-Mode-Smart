#include "motor.h"

unsigned char speed_value = 180;

void motor_init(void) {
    DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
    PORTD = 0x00;

    // Timer0 PWM - ENA PB3
    DDRB |= (1 << PB3);
    TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS00);
    OCR0 = speed_value;

    // Timer2 PWM - ENB PD7
    DDRD |= (1 << PD7);
    TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (1 << CS20);
    OCR2 = speed_value;
}

void motor_control(unsigned char in1, unsigned char in2,
                   unsigned char in3, unsigned char in4) {
    PORTD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));
    if (in1) PORTD |= (1 << PD2);
    if (in2) PORTD |= (1 << PD3);
    if (in3) PORTD |= (1 << PD4);
    if (in4) PORTD |= (1 << PD5);
}

void set_speed(unsigned char sp) {
    speed_value = sp;
    OCR0 = sp;
    OCR2 = sp;
}
