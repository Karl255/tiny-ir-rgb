#include <control.h>

bool is_on = false; // TODO: load from EEPROM
Mode mode = Mode::static_; // TODO: load from EEPROM
Mode previous_mode = Mode::none;
uint8_t brightness = BRIGHTNESS_MAX;

static inline uint8_t applyBrightness(uint8_t val) {
	return val * brightness / BRIGHTNESS_MAX;
}

void setOutputColors(uint8_t r, uint8_t g, uint8_t b) {
	PWM_R = applyBrightness(r);
	PWM_G = applyBrightness(g);
	PWM_B = applyBrightness(b);
}
