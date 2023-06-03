/*
MIT License

Copyright (c) 2023 Karlo B.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <ir_NEC.h>
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
			// timeout
			return;
		}
		
		_delay_us(IR_T_UNIT_US / 10.0);
		sync_counter++;
	} while (_read() == 0);
	
	// u : base NEC communication time unit in IR_T_UNIT_US / 10.0 units
	const uint8_t u = sync_counter / 16;
	uint8_t counter;
	
	// transmission start, 8u high pulse
	// repeat start, 4u high pulse
	counter = 0;
	do {
		if (counter == UINT8_MAX) {
			// timeout
			return;
		}
		
		_delay_us(IR_T_UNIT_US / 10.0);
		counter++;
	} while (_read() == 1);
	
	if (counter <= 6 * u) {
		// skip 1u low pulse with 1u padding
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
					// timeout
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
					// timeout
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
	
	// skip 1u low pulse with 1u padding
	for (uint8_t i = 0; i < 2 * u; i++) {
		_delay_us(IR_T_UNIT_US / 10.0);
	}
	
	if (payload[0] != (uint8_t)~payload[1] || payload[2] != (uint8_t)~payload[3]) {
		// communication error; discard data
		return;
	}
	
	// return the decoded command code
	ir_readout.command = (Command)payload[2];
	
	ir_readout.status = IrStatus::transmission;
	return;
	
	#undef _read
}

#undef IR_T_UNIT_US
