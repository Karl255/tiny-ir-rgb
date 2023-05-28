#pragma once
#include <stdint.h>

namespace Static {
	extern uint8_t r;
	extern uint8_t g;
	extern uint8_t b;
	
	// TODO: check if this is actually needed
	void init(void);
	void tick(void);
	void set(uint8_t r, uint8_t g, uint8_t b);
}
