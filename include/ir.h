#pragma once

#include <control.h>
#include <stdint.h>

enum class IrStatus : uint8_t {
	none = 0,
	transmission = 1,
	repeat = 2,
};

struct IrReadout {
	volatile IrStatus status;
	volatile Command command;
};

extern volatile struct IrReadout ir_readout;
