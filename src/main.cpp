#include <avr/io.h>
#include <avr/interrupt.h>
#include <init.h>
#include <ir_NEC.h>
#include <control.h>
#include <mode_static.h>
#include <mode_rg_cycle.h>
#include <mode_rgw_cycle.h>
#include <mode_hue_cycle.h>

// in ~0.5ms units
volatile uint16_t now = 0;
uint16_t last_smooth_tick = 0;
uint16_t last_mode_tick = 0;

int main(void) {
	init();
	init_control();
	sei();
	
	for (;;) {
		if (ir_readout.status == IrStatus::transmission && (is_on || ir_readout.command == Command::on)) {
			ir_readout.status = IrStatus::none;
			
			switch (ir_readout.command) {
				case Command::on:
					if (!is_on) {
						setIsOn(true);
					}
						
					break;
				
				case Command::off:
					if (is_on) {
						setIsOn(false);
					}
					
					break;
				
				case Command::reset:
					setBrightness(BRIGHTNESS_MAX);
					previous_mode = Mode::none;
					break;
				
				case Command::increment:
					if (brightness < BRIGHTNESS_MAX) {
						setBrightness(brightness + 1);
					}
					
					break;
				
				case Command::decrement:
					if (brightness > BRIGHTNESS_MIN) {
						setBrightness(brightness - 1);
					}
					
					break;
				
				case Command::rg_cycle:
					setMode(Mode::rg_cycle);
					break;
				
				case Command::rgw_cycle:
					setMode(Mode::rgw_cycle);
					break;
				
				case Command::hue_cycle:
					setMode(Mode::hue_cycle);
					break;
				
				case Command::static_W   : setMode(Mode::static_); Static::set(255, 255, 255); break;
				case Command::static_R   : setMode(Mode::static_); Static::set(255,   0,   0); break;
				case Command::static_RRRG: setMode(Mode::static_); Static::set(255, 102,   0); break;
				case Command::static_RRG : setMode(Mode::static_); Static::set(255, 204,   0); break;
				case Command::static_RGG : setMode(Mode::static_); Static::set(204, 255,   0); break;
				case Command::static_RGGG: setMode(Mode::static_); Static::set(102, 255,   0); break;
				case Command::static_G   : setMode(Mode::static_); Static::set(  0, 255,   0); break;
				case Command::static_GGGB: setMode(Mode::static_); Static::set(  0, 255, 102); break;
				case Command::static_GGB : setMode(Mode::static_); Static::set(  0, 255, 204); break;
				case Command::static_GBB : setMode(Mode::static_); Static::set(  0, 204, 255); break;
				case Command::static_GBBB: setMode(Mode::static_); Static::set(  0, 102, 255); break;
				case Command::static_B   : setMode(Mode::static_); Static::set(  0,   0, 255); break;
				case Command::static_BBBR: setMode(Mode::static_); Static::set(102,   0, 255); break;
				case Command::static_BBR : setMode(Mode::static_); Static::set(204,   0, 255); break;
				case Command::static_BRR : setMode(Mode::static_); Static::set(255,   0, 204); break;
				case Command::static_BRRR: setMode(Mode::static_); Static::set(255,   0, 102); break;
				
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
					
					case Mode::rg_cycle:
						RgCycle::init();
						break;
					
					case Mode::rgw_cycle:
						RgwCycle::init();
						break;
					
					case Mode::hue_cycle:
						HueCycle::init();
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
				
				case Mode::rg_cycle:
					if (now - last_mode_tick >= CYCLE_TICK_TIME_STEP) {
						last_mode_tick = now;
						RgCycle::tick();
					}
				
				case Mode::rgw_cycle:
					if (now - last_mode_tick >= CYCLE_TICK_TIME_STEP) {
						last_mode_tick = now;
						RgwCycle::tick();
					}
					
					break;
				
				case Mode::hue_cycle:
					if (now - last_mode_tick >= CYCLE_TICK_TIME_STEP) {
						last_mode_tick = now;
						HueCycle::tick();
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
