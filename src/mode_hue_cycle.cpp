#include <stdint.h>
#include <control.h>
#include <mode_hue_cycle.h>

// 8 bytes reserved
#define EEPROM_START 16

uint8_t HueCycle::step;
uint8_t HueCycle::r;
uint8_t HueCycle::g;
uint8_t HueCycle::b;

void HueCycle::init(void) {
	step = 0;
	r = 255;
	g = 0;
	b = 0;
}

void HueCycle::tick(void) {
	switch (step) {
		case 0: g++; if (g == 255) { step++; } break; // to yellow
		case 1: r--; if (r == 0)   { step++; } break; // to green
		case 2: b++; if (b == 255) { step++; } break; // to cyan
		case 3: g--; if (g == 0)   { step++; } break; // to blue
		case 4: r++; if (r == 255) { step++; } break; // to purple
		case 5: b--; if (b == 0)   { step++; } break; // to red
		
		default:
			step = 0;
			break;
	}
	
	if (step > 5) {
		step = 0;
	}
	
	setOutputColors(
		applyBrightness(r),
		applyBrightness(g),
		applyBrightness(b)
	);
}
