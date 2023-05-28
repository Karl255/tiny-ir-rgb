#include <control.h>

bool is_on = false; // TODO: load from EEPROM
Mode mode = Mode::static_; // TODO: load from EEPROM
Mode previous_mode = Mode::none;

uint8_t brightness = BRIGHTNESS_MAX;
uint8_t target_r;
uint8_t target_g;
uint8_t target_b;

void setOutputColors(uint8_t r, uint8_t g, uint8_t b) {
	setOutputSmooth(r, g, b);
	PWM_R = applyBrightness(r);
	PWM_G = applyBrightness(g);
	PWM_B = applyBrightness(b);
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
