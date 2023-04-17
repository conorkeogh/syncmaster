
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
	newData = false;
	idx = 0;

	// Wait until message received
	while (!Serial.available()) {}

	// Read in message
	while (Serial.available() && newData == false) {
		// Read in next byte
		current_byte = Serial.read();

		// Check if end of current string
		if (current_byte != endMarker) {
			// Save current byte
			message[idx] = current_byte;
			idx++;

			// Avoid overflow
			if (idx >= 32) { idx = 31; }
		}
		else {
			// Save end of message
			message[idx] = '\0';
			newData = true;
		}
	}

	// Parse received message
	parseMessage();
}

/* Start signal
   Send signal to indicate start of trial */
void SyncMaster::start() {
	// Send signal
	sendPulse(STARTPULSE);
  Serial.println("start");
}

/* End signal
   Send signal to indicate start of trial */
void SyncMaster::end() {
	// Send signal
	sendPulse(ENDPULSE);
   Serial.println("end");
}

/* Event 1 signal
   Send signal to indicate event 1 */
void SyncMaster::event1() {
	// Send signal
	sendPulse(EVENT1PULSE);
  Serial.println("event1");
}

/* Event 2 signal
   Send signal to indicate event 2 */
void SyncMaster::event2() {
	// Send signal
	sendPulse(EVENT2PULSE);
  Serial.println("event2");
}

/* Parse input
   Interpret received message and trigger response */
void SyncMaster::parseMessage() {
	// Get input message
	int message_parsed = atoi(message);

	// Parse input and respond
	if (message_parsed == STARTMARKER) { start(); }
	else if (message_parsed == ENDMARKER) { end(); }
	else if (message_parsed == EVENT1) { event1(); }
	else if (message_parsed == EVENT2) { event2(); }
	else if (message_parsed == HOST_MESSAGE) { acknowledge(); }
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

/* Acknowledge host message
   If host message received, respond with acknowledge signal
   Allows device to be automatically identified by host drivers */
void SyncMaster::acknowledge() {
	// Send acknowledge message over serial
	Serial.println(ACKNOWLEDGE);
}
