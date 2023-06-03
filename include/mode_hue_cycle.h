#pragma once
#include <stdint.h>

namespace HueCycle {
	extern uint8_t step;
	extern uint8_t r;
	extern uint8_t g;
	extern uint8_t b;
	
	void init(void);
	void tick(void);
}
