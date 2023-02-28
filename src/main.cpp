#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB = 1 << PB4 | 1 << PB1 | 1 << PB0;
	
	TCCR0A =
		0b10 << COM0A0 | // enable PWM on OC0A, non-inverting mode
		0b10 << COM0B0 | // enable PWM on OC0B, non-inverting mode
		0b01 << WGM00; // phase correct PWM mode (halves frequency)
	
	TCCR0B =
		0 << WGM02 | // count to 0xFF
		0b010 << CS00; // CLK / 2 / 8 = ~2 Hz PWM
	
	TCCR1 =
		0b0101 << CS10; // CLK / 16 = ~2 kHz PWM

	GTCCR =
		1 << PWM1B |
		0b10 << COM1B0; // PWM mode

	OCR0A = 0;
	OCR0B = 0;
	OCR1B = 0;
	
	for (;;) {
		do {
			OCR0A++;
			_delay_ms(4);
		} while (OCR0A);
		OCR0A = 0;
		
		do {
			OCR0B++;
			_delay_ms(4);
		} while (OCR0B);
		
		do {
			OCR1B++;
			_delay_ms(4);
		} while (OCR1B);
	}
	
	return 0;
}
