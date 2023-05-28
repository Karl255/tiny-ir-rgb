#pragma once

#include <stdint.h>
#include <avr/io.h>

#define PWM_R OCR0B
#define PWM_G OCR1B
#define PWM_B OCR0A

#define BRIGHTNESS_MAX 10
#define BRIGHTNESS_MIN 1
#define SMOOTH_TICK_TIME_STEP 2
#define STATIC_TICK_TIME_STEP 100

enum class Mode : uint8_t {
	none = 0,
	static_ = 1,
	cycle_rg = 2,
	cycle_rgw = 3,
	cycle_hue = 4
};

extern bool is_on;
extern Mode mode;
extern Mode previous_mode;

extern uint8_t brightness;
extern uint8_t target_r;
extern uint8_t target_g;
extern uint8_t target_b;

/*
05 04 06 07 | +    -    OFF  ON
09 08 0a 0b | R    G    B    W
0d 0c 0e 0f | RRRG GGGB BBBR RST/ALT
15 14 16 17 | RRG  GGB  BBR  R-G
19 18 1a 1b | RGG  GBB  BRR  R-G-W
11 10 12 13 | RGGG GBBB BRRR HUE
*/

enum class Command : uint8_t {
	off = 0x06,
	on = 0x07,
	increment = 0x05,
	decrement = 0x04,
	reset_alternate = 0x0f,
	
	rg_cycle = 0x17,
	rgw_cycle = 0x1b,
	hue_cycle = 0x13,
	
	static_R = 0x09,
	static_RRRG = 0x0d,
	static_RRG = 0x15,
	static_RGG = 0x19,
	static_RGGG = 0x11,
	
	static_G = 0x08,
	static_GGGB = 0x0c,
	static_GGB = 0x14,
	static_GBB = 0x18,
	static_GBBB = 0x10,
	
	static_B = 0x0a,
	static_BBBR = 0x0e,
	static_BBR = 0x16,
	static_BRR = 0x1a,
	static_BRRR = 0x12,
	
	static_W = 0x0b,
};

void setOutputColors(uint8_t r, uint8_t g, uint8_t b);
void setOutputSmooth(uint8_t r, uint8_t g, uint8_t b);
void tickSmoothOutput();

inline uint8_t applyBrightness(uint8_t val) {
	return val * brightness / BRIGHTNESS_MAX;
}
