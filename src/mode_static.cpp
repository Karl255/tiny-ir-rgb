#include <stdint.h>
#include <control.h>
#include <mode_static.h>

uint8_t Static::r;
uint8_t Static::g;
uint8_t Static::b;

void Static::init(void) { }
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
}
