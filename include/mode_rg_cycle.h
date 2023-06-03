#pragma once
#include <stdint.h>

namespace RgCycle {
	extern uint8_t step;
	extern uint8_t r;
	extern uint8_t g;
	extern uint8_t wait;
	
	void init(void);
	void tick(void);
}
