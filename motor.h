#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>

extern unsigned char speed_value;

void motor_init(void);
void motor_control(unsigned char in1, unsigned char in2,
                   unsigned char in3, unsigned char in4);
void set_speed(unsigned char sp);

#endif
