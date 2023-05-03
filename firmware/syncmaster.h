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

/* Define messages to respond to */
#define STARTMARKER 1
#define ENDMARKER 2
#define EVENT1 3
#define EVENT2 4

/* Define trigger pulse width (milliseconds) */
#define STARTPULSE 50
#define ENDPULSE 100
#define EVENT1PULSE 150
#define EVENT2PULSE 200

/* Define host acknowledge messages */
#define HOST_MESSAGE 5
#define ACKNOWLEDGE 42

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
		// Send triggers
		void start();
		void end();
		void event1();
		void event2();

		void parseMessage();	// Respond based on message received
		void sendPulse(int);	// Send output signal
		void acknowledge();	// Acknowledge message from host

		// Message array
		char message[32] = { 0 };

		// Set end marker
		char endMarker = '\n';

		// Track current state
		bool newData = false;
		int idx = 0;
		char current_byte = 0;
};

#endif
