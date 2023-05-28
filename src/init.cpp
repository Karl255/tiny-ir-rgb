#include <avr/interrupt.h>
#include <init.h>
#include <control.h>

void init(void) {
	DDRB =
		1 << PB4 | // output for OC1B; G
		0 << PB2 | // input with INT0 for IR receiver
		1 << PB1 | // output for OC0B; R
		1 << PB0; // output for OC0A; B
	
	TCCR0A =
		0b10 << COM0A0 | // PWM on OC0A, non-inverting mode
		0b10 << COM0B0 | // PWM on OC0B, non-inverting mode
		0b01 << WGM00; // phase correct PWM mode (halves frequency)
	
	TCCR0B =
		0 << WGM02 | // count to 0xFF
		0b010 << CS00; // CLK / 8 (/ 2) = ~2 Hz PWM
	
	TCCR1 = 0b0101 << CS10; // CLK / 16 = ~2 kHz PWM
	GTCCR = 1 << PWM1B | 0b10 << COM1B0; // PWM mode

	PWM_R = 0; // OCR0B
	PWM_G = 0; // OCR1B
	PWM_B = 0; // OCR0A
	
	MCUCR = 0b10 << ISC00; // INT0 triggers on falling edge
	GIMSK = 1 << INT0; // enable INT0
	
	TIMSK = 1 << TOIE1; // timer 1 overflow interrupt
}
