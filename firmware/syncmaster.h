/*	SyncMaster device driver

	Oxford Neural Interfacing
	Written by Conor Keogh
	conor.keogh@nds.ox.ac.uk
	16/04/2023
*/

// Ensure library only included once
#ifndef syncmaster_h
#define syncmaster_h

/* Include base library */
#include <Arduino.h>

/* Define physical pin IDs */
#define OUTPUT_PIN 5

/* Define pulse step size and max pulse width (ms) */
#define PULSE_STEPSIZE 10
#define MAX_PULSE 1000

/* Define COM port settings */
#define BAUDRATE 115200

// Library interface
class SyncMaster {
	// Public interface
	public:
		SyncMaster();	// Constructor
		void init();	// Initialise hardware
		void run();	// Run device

	// Private interface
	private:
		void parseMessage();	// Respond based on message received
		void sendPulse(int);	// Send output signal

		// Message array
		const byte numChars = 32;
		char message[32] = { 0 };
		int dataNumber = 0;

		// Acknowledge messages
		char *greeting = "best wishes";
		char *response = "warmest regards";

		// Set markers
		char startMarker = '<';
		char endMarker = '>';

		// Track current state
		bool newData = false;
};

#endif
