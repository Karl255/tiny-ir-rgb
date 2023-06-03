#include <avr/eeprom.h>
#include <stdint.h>
#include <control.h>
#include <mode_static.h>

// 8 bytes reserved
#define EEPROM_START 8

uint8_t Static::r;
uint8_t Static::g;
uint8_t Static::b;

void Static::init(void) {
	Static::r = eeprom_read_byte((const uint8_t*)(EEPROM_START + 0));
	Static::g = eeprom_read_byte((const uint8_t*)(EEPROM_START + 1));
	Static::b = eeprom_read_byte((const uint8_t*)(EEPROM_START + 2));
}

void Static::tick(void) {
	setOutputSmooth(
		applyBrightness(Static::r),
		applyBrightness(Static::g),
		applyBrightness(Static::b)
	);
}

void Static::set(uint8_t r, uint8_t g, uint8_t b) {
	Static::r = r;
	Static::g = g;
	Static::b = b;
	
	_EEPUT(EEPROM_START + 0, r);
	_EEPUT(EEPROM_START + 1, g);
	_EEPUT(EEPROM_START + 2, b);
}

#undef EEPROM_START
