#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#include "lcd.h"
#include "motor.h"
#include "uart.h"
#include "ultrasonic.h"
#include "ir.h"

unsigned char mode = 0;

int main(void) {
    // Init
    motor_init();
    ultrasonic_init();
    ir_init();
    uart_init();
    lcd_init();

    // Buzzer
    DDRC |= (1 << PC4);
    PORTC &= ~(1 << PC4);

    OCR0 = 180;
    OCR2 = 180;

    lcd_clear();
    lcd_goto(0, 1);
    lcd_print(" Multi | Mode ");
    lcd_goto(1, 0);
    lcd_print(" Smart Car Robot ");

    while(!uart_available());

    unsigned char first_cmd;
    while(1) {
        if (uart_available()) {
            first_cmd = uart_read();
            if (first_cmd == 'M' || first_cmd == 'm') { mode = 0; break; }
            if (first_cmd == 'A' || first_cmd == 'a') { mode = 1; break; }
            if (first_cmd == 'X' || first_cmd == 'x') { mode = 2; break; }
        }
    }

    unsigned char cmd;
    unsigned int dist;
    unsigned char prev_mode = 99;

    while(1) {

        // ===== Manual Mode =====
        if (mode == 0) {
            if (prev_mode != 0) {
                prev_mode = 0;
                lcd_clear();
                lcd_goto(0, 0);
                lcd_print("Mode: Manual");
                lcd_goto(1, 0);
                lcd_print("Choose Direction");
            }

            if (uart_available()) {
                cmd = uart_read();
                switch (cmd) {
                    case 'A': case 'a':
                        mode = 1;
                        motor_control(0, 0, 0, 0);
                        set_speed(180);
                        break;
                    case 'X': case 'x':
                        mode = 2;
                        motor_control(0, 0, 0, 0);
                        set_speed(185);
                        break;
                    case 'F': case 'f':
                        motor_control(1, 0, 1, 0);
                        break;
                    case 'B': case 'b':
                        motor_control(0, 1, 0, 1);
                        break;
                    case 'R': case 'r':
                        motor_control(0, 0, 1, 0);
                        break;
                    case 'L': case 'l':
                        motor_control(1, 0, 0, 0);
                        break;
                    case 'S': case 's':
                        motor_control(0, 0, 0, 0);
                        break;
                    case '0': set_speed(0);   break;
                    case '1': set_speed(64);  break;
                    case '2': set_speed(128); break;
                    case '3': set_speed(192); break;
                    case '4': set_speed(255); break;
                    case '+':
                        if (speed_value < 245)
                            set_speed(speed_value + 10);
                        break;
                    case '-':
                        if (speed_value > 10)
                            set_speed(speed_value - 10);
                        break;
                }
            }
        }

        // ===== Auto Mode =====
        else if (mode == 1) {
            if (prev_mode != 1) {
                prev_mode = 1;
                lcd_clear();
                lcd_goto(0, 0);
                lcd_print("Mode 2:");
                lcd_goto(1, 0);
                lcd_print("Auto Avoid");
            }

            if (uart_available()) {
                cmd = uart_read();
                if (cmd == 'M' || cmd == 'm') {
                    mode = 0;
                    motor_control(0, 0, 0, 0);
                    PORTC &= ~(1 << PC4);
                    set_speed(180);
                    continue;
                }
                if (cmd == 'X' || cmd == 'x') {
                    mode = 2;
                    motor_control(0, 0, 0, 0);
                    PORTC &= ~(1 << PC4);
                    set_speed(185);
                    continue;
                }
            }

            dist = get_distance();
            _delay_ms(50);

            if (dist < 20)
                PORTC |= (1 << PC4);
            else
                PORTC &= ~(1 << PC4);

            if (dist > 30) {
                motor_control(1, 0, 1, 0);
            } else {
                motor_control(0, 0, 0, 0);
                _delay_ms(300);
                motor_control(0, 1, 0, 1);
                _delay_ms(400);
                motor_control(0, 0, 0, 0);
                _delay_ms(300);
                motor_control(0, 0, 1, 0);
                _delay_ms(500);
                motor_control(0, 0, 0, 0);
                _delay_ms(300);

                dist = get_distance();

                if (dist > 30) {
                    motor_control(1, 0, 1, 0);
                } else {
                    motor_control(1, 0, 0, 0);
                    _delay_ms(1000);
                    motor_control(0, 0, 0, 0);
                    _delay_ms(300);
                    motor_control(1, 0, 1, 0);
                }
            }
        }

        // ===== Line Following Mode =====
        else if (mode == 2) {
            if (prev_mode != 2) {
                prev_mode = 2;
                lcd_clear();
                lcd_goto(0, 0);
                lcd_print("Mode 3:");
                lcd_goto(1, 0);
                lcd_print("Line Following");
            }

            if (uart_available()) {
                cmd = uart_read();
                if (cmd == 'M' || cmd == 'm') {
                    mode = 0;
                    motor_control(0, 0, 0, 0);
                    set_speed(190);
                    continue;
                }
                if (cmd == 'A' || cmd == 'a') {
                    mode = 1;
                    motor_control(0, 0, 0, 0);
                    set_speed(180);
                    continue;
                }
            }

            if (ir_left() == 1 && ir_right() == 1) {
                motor_control(1, 0, 1, 0);
            }
            else if (ir_left() == 0 && ir_right() == 1) {
                motor_control(0, 0, 1, 0);
            }
            else if (ir_left() == 1 && ir_right() == 0) {
                motor_control(1, 0, 0, 0);
            }
            else {
                motor_control(0, 0, 0, 0);
            }
        }
    }
}
