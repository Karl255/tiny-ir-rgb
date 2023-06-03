#include <avr/eeprom.h>
#include <control.h>

// 8 bytes reserved
#define EEPROM_START 0

bool is_on = false;
Mode mode = Mode::static_;
Mode previous_mode = Mode::none;

uint8_t brightness = BRIGHTNESS_MAX;
uint8_t target_r;
uint8_t target_g;
uint8_t target_b;

void init_control(void) {
	_EEGET(is_on, EEPROM_START + 0);
	mode = (Mode)eeprom_read_byte((const uint8_t*)(EEPROM_START + 1));
	previous_mode = Mode::none;
	_EEGET(brightness, EEPROM_START + 2);
	
	if (brightness < BRIGHTNESS_MIN || brightness > BRIGHTNESS_MAX) {
		brightness = BRIGHTNESS_MAX;
	}
}

void setIsOn(bool value) {
	_EEPUT(EEPROM_START + 0, value);
	is_on = value;
}

void setMode(Mode value) {
	_EEPUT(EEPROM_START + 1, value);
	mode = value;
}

void setBrightness(uint8_t value) {
	_EEPUT(EEPROM_START + 2, value);
	brightness = value;
}

void setOutputColors(uint8_t r, uint8_t g, uint8_t b) {
	r = applyBrightness(r);
	g = applyBrightness(g);
	b = applyBrightness(b);
	
	setOutputSmooth(r, g, b);
	
	PWM_R = r;
	PWM_G = g;
	PWM_B = b;
}

void setOutputSmooth(uint8_t r, uint8_t g, uint8_t b) {
	target_r = r;
	target_g = g;
	target_b = b;
}

static int8_t delta(uint8_t value, uint8_t target) {
	if (target > value) {
		return 1;
	} else if (target < value) {
		return -1;
	} else {
		return 0;
	}
}

void tickSmoothOutput() {
	PWM_R += delta(PWM_R, target_r);
	PWM_G += delta(PWM_G, target_g);
	PWM_B += delta(PWM_B, target_b);
}

#undef EEPROM_START
