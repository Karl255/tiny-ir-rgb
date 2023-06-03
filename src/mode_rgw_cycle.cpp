#include <stdint.h>
#include <control.h>
#include <mode_rgw_cycle.h>

// 8 bytes reserved
#define EEPROM_START 16

uint8_t RgwCycle::step;
uint8_t RgwCycle::r;
uint8_t RgwCycle::g;
uint8_t RgwCycle::b;
uint8_t RgwCycle::wait;

void RgwCycle::init(void) {
	step = 0;
	r = 255;
	g = 0;
	b = 0;
	wait = 0;
}

void RgwCycle::tick(void) {
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
		
		// to white
		case 4:
			r++;
			b++;
			if (r == 255) { step++; }
			break;
		
		// hold white
		case 5:
			wait++;
			
			if (wait == 40) {
				wait = 0;
				step++;
			}
			
			break;
		
		// to red
		case 6:
			g--;
			b--;
			if (g == 0) { step++; }
			break;
		
		default:
			step = 0;
			break;
	}
	
	if (step > 6) {
		step = 0;
	}
	
	setOutputColors(
		applyBrightness(r),
		applyBrightness(g),
		applyBrightness(b)
	);
}
