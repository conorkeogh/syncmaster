
/*	SyncMaster device driver

	Oxford Neural Interfacing
	Written by Conor Keogh
	conor.keogh@nds.ox.ac.uk
	16/04/2023
*/

#include "syncmaster.h"

/* Constructor */
SyncMaster::SyncMaster() {
	// No constructor
}

/* Initialise
   Set up serial communication and set pins */
void SyncMaster::init() {
	// Initialise serial
	Serial.begin(BAUDRATE);

	// Set target pin as output
	pinMode(OUTPUT_PIN, OUTPUT);

	// Set output to low
	digitalWrite(OUTPUT_PIN, LOW);
}

/* Run device
   Wait for input on serial port
   When received, record message then parse and respond */
void SyncMaster::run() {
	// Reset state
	static boolean recvInProgress = false;
	static byte idx = 0;
	char current_byte = 0;

	// Read in message
	while (Serial.available() && newData == false) {
		// Read in next byte
		current_byte = Serial.read();

		// Check if currently receiving data (i.e. start indicator received)
		if (recvInProgress == true) {
			// Record byte if not end of message
			if (current_byte != endMarker) {
				// Save to message array
				message[idx] = current_byte;

				// Move to next index in message array
				idx++;

				// Avoid overflowing array
				if (idx >= numChars) { idx = numChars - 1; }
			}
			
			// If end of message: end terminate message
			else {
				// Terminate message
				message[idx] = '\0';
				recvInProgress = false;
				idx = 0;
				newData = true;
			}
		}

		// If start marker received: start saving to array
		else if (current_byte = startMarker) {
			recvInProgress = true;
		}
	}

	// If new data received: parse this and reset
	if (newData == true) {
		// Convert message to integer
		dataNumber = 0;
		dataNumber = atoi(message);

		// Parse received message
		parseMessage();

		// Reset data flag
		newData = false;
	}
}

/* Parse input
   Interpret received message and trigger response */
void SyncMaster::parseMessage() {

	// Map number -> pulse duration
	int pulse_duration = dataNumber * PULSE_STEPSIZE;

	// If target duration less than max, send pulse
	if (pulse_duration <= MAX_PULSE) {
		sendPulse(pulse_duration);
	}

	if (strcmp(message, greeting) == 0) {
		Serial.println(response);
	}
}

/* Send output signal
   Sets target pin high for specified duration */
void SyncMaster::sendPulse(int pulsewidth) {
	// Set pin high
	digitalWrite(OUTPUT_PIN, HIGH);

	// Wait for specified duration
	delay(pulsewidth);

	// Set pin low
	digitalWrite(OUTPUT_PIN, LOW);
}
