#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <init.h>
#include <ir.h>
#include <control.h>
#include <mode_static.h>

// in ~0.5ms units
volatile uint16_t now = 0;
uint16_t last_smooth_tick = 0;
uint16_t last_mode_tick = 0;

int main(void) {
	init();
	sei();
	
	for (;;) {
		if (ir_readout.status == IrStatus::transmission && (is_on || ir_readout.command == Command::on)) {
			ir_readout.status = IrStatus::none;
			
			switch (ir_readout.command) {
				case Command::on:
					if (!is_on) {
						brightness = BRIGHTNESS_MAX;
						is_on = true;
					}
						
					break;
				
				case Command::off:
					if (is_on) {
						is_on = false;
					}
					
					break;
				
				case Command::reset_alternate:
					brightness = BRIGHTNESS_MAX;
					previous_mode = Mode::none;
					break;
				
				case Command::increment:
					if (brightness < BRIGHTNESS_MAX) {
						brightness++;
					}
					
					break;
				
				case Command::decrement:
					if (brightness > BRIGHTNESS_MIN) {
						brightness--;
					}
					
					break;
				
				case Command::rg_cycle:
					break;
				
				case Command::rgw_cycle:
					break;
				
				case Command::hue_cycle:
					break;
				
				case Command::static_W   : mode = Mode::static_; Static::set(255, 255, 255); break;
				case Command::static_R   : mode = Mode::static_; Static::set(255,   0,   0); break;
				case Command::static_RRRG: mode = Mode::static_; Static::set(255, 102,   0); break;
				case Command::static_RRG : mode = Mode::static_; Static::set(255, 204,   0); break;
				case Command::static_RGG : mode = Mode::static_; Static::set(204, 255,   0); break;
				case Command::static_RGGG: mode = Mode::static_; Static::set(102, 255,   0); break;
				case Command::static_G   : mode = Mode::static_; Static::set(  0, 255,   0); break;
				case Command::static_GGGB: mode = Mode::static_; Static::set(  0, 255, 102); break;
				case Command::static_GGB : mode = Mode::static_; Static::set(  0, 255, 204); break;
				case Command::static_GBB : mode = Mode::static_; Static::set(  0, 204, 255); break;
				case Command::static_GBBB: mode = Mode::static_; Static::set(  0, 102, 255); break;
				case Command::static_B   : mode = Mode::static_; Static::set(  0,   0, 255); break;
				case Command::static_BBBR: mode = Mode::static_; Static::set(102,   0, 255); break;
				case Command::static_BBR : mode = Mode::static_; Static::set(204,   0, 255); break;
				case Command::static_BRR : mode = Mode::static_; Static::set(255,   0, 204); break;
				case Command::static_BRRR: mode = Mode::static_; Static::set(255,   0, 102); break;
				
				default:
					break;
			}
		}
		
		if (is_on) {
			if (mode != previous_mode) {
				previous_mode = mode;

				switch (mode) {
					case Mode::static_:
						Static::init();
						break;
				
					default:
						break;
				}
			}
		
			switch (mode) {
				case Mode::static_:
					if (now - last_mode_tick >= STATIC_TICK_TIME_STEP) {
						last_mode_tick = now;
						Static::tick();
					}
					
					break;
				
				default:
					break;
			}
		} else {
			setOutputSmooth(0, 0, 0);
		}
		
		if (now - last_smooth_tick >= SMOOTH_TICK_TIME_STEP) {
			last_smooth_tick = now;
			tickSmoothOutput();
		}
	}
	
	return 0;
}

ISR(TIMER1_OVF_vect) {
	now++;
}
