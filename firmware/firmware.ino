/*	SyncMaster device firmware

	Oxford Neural Interfacing
	Written by Conor Keogh
	conor.keogh@nds.ox.ac.uk
	16/04/2023

	Monitors serial ports
	Sends trigger pulse over output port based on incoming message
	Responses and ports set in configuration.h
*/

// Import drivers
#include "syncmaster.h"

// Instantiate device object
SyncMaster syncmaster;

// Initialise
void setup() {
	// Initialise hardware
	syncmaster.init();
}

// Run main loop
void loop() {
	// Run device
	syncmaster.run();
}
