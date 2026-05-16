#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void ultrasonic_init(void);
unsigned int get_distance(void);

#endif
