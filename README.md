# tiny-ir-rgb

An IR (NEC) RGB LED strip controller powered by an ATtiny45/85.

Features:
- color-cycle animations (easily customisable)
- remembers state in EEPROM - to instantly resume after a power loss
- NEC infrared remote control (custom library)
	- reliable operation even with the internal RC oscillator
- low MCU program memory requirements - 2458 bytes

Problems:
- the NEC IR code blocks during the whole transmission (67.5ms); a rewrite to fix this is planned in the future

Feel free to use the NEC IR code and adjust it for your own purpose.

Dependencies: none.

This project is made using [PlatformIO](https://platformio.org/).
