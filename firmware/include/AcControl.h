#pragma once

namespace AcControl {

// Initialize IR control without transmitting a command.
void begin();

// Transmit an ON request using configured settings; physical state is unconfirmed.
void sendOn();

// Transmit an OFF request; this does not confirm the physical AC state.
void sendOff();

}
