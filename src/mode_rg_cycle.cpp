#include <stdint.h>
#include <control.h>
#include <mode_rg_cycle.h>

// 8 bytes reserved
#define EEPROM_START 16

uint8_t RgCycle::step;
uint8_t RgCycle::r;
uint8_t RgCycle::g;
uint8_t RgCycle::wait;

void RgCycle::init(void) {
	step = 0;
	r = 255;
	g = 0;
	wait = 0;
}

void RgCycle::tick(void) {
	switch (step) {
		// hold red
		case 0:
			wait++;
			
			if (wait == 40) {
				wait = 0;
				step++;
			}
			
			break;
		
		// to yellow
		case 1:
			g++;
			if (g == 255) { step++; }
			break;
		
		// to green
		case 2:
			r--;
			if (r == 0) { step++; }
			break;
		
		// hold green
		case 3:
			wait++;
			
			if (wait == 40) {
				wait = 0;
				step++;
			}
			
			break;
		
		// to yellow
		case 4:
			r++;
			if (r == 255) { step++; }
			break;
		
		// to red
		case 5:
			g--;
			if (g == 0) { step++; }
			break;
		
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
		0
	);
}
