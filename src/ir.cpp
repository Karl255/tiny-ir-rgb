#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <ir.h>
#include <control.h>

#define IR_T_UNIT_US 562.5

volatile struct IrReadout ir_readout;

ISR(INT0_vect) {
	#define _read() ((PINB >> PB2) & 1)
	
	uint16_t sync_counter = 0;
	
	// transmission/repeat start 16u low pulse
	do {
		// 1300 * u/10 = 72.125ms (67.5ms total transmission)
		if (sync_counter == 1300) {
			return;
		}
		
		_delay_us(IR_T_UNIT_US / 10.0);
		sync_counter++;
	} while (_read() == 0);
	
	// u : base NEC communication time unit in IR_T_UNIT_US / 10.0 units
	const uint8_t u = sync_counter / 16;
	uint8_t counter;
	
	// transmission start 8u high pulse
	// repeat start 4u high pulse
	counter = 0;
	do {
		if (counter == UINT8_MAX) {
			return;
		}
		
		_delay_us(IR_T_UNIT_US / 10.0);
		counter++;
	} while (_read() == 1);
	
	if (counter <= 6 * u) {
		// skip 1u low pulse + 1u padding
		for (uint8_t i = 0; i < 2 * u; i++) {
			_delay_us(IR_T_UNIT_US / 10.0);
		}
		
		ir_readout.status = IrStatus::repeat;
		return;
	}
	
	uint8_t payload[4];
	for (uint8_t i = 0; i < 4; i++) {
		uint8_t b = 0;
		
		for (uint8_t _j = 0; _j < 8; _j++) {
			// 1u low pulse (both "0" and "1")
			counter = 0;
			do {
				if (counter == UINT8_MAX) {
					return;
				}
				
				_delay_us(IR_T_UNIT_US / 10.0);
				counter++;
			} while(_read() == 0);
			
			// "0", 1u
			// "1", 3u
			counter = 0;
			do {
				if (counter == UINT8_MAX) {
					return;
				}
				
				_delay_us(IR_T_UNIT_US / 10.0);
				counter++;
			} while(_read() == 1);
			
			b >>= 1;
			
			if (counter > 2 * u) {
				b |= 0x80;
			}
		}
		
		payload[i] = b;
	}
	
	// skip 1u low pulse + 1u padding
	for (uint8_t i = 0; i < 2 * u; i++) {
		_delay_us(IR_T_UNIT_US / 10.0);
	}
	
	if (payload[0] != (uint8_t)~payload[1] || payload[2] != (uint8_t)~payload[3]) {
		// communication error; discard data
		return;
	}
	
	// "return" command
	ir_readout.command = (Command)payload[2];
	
	ir_readout.status = IrStatus::transmission;
	return;
	
	#undef _read
}

#undef IR_T_UNIT_US
