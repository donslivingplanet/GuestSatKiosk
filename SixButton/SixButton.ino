//=============================================================================
/*!
	@title: SixButton
	@summary:

	@copyright: (c) 2015 LLPA. ALL RIGHTS RESERVED
	@version: 1.0

	@organization: LLPA
	@author:  Gregory L. Bodine
	@created: 2015-01-10

	@eeprom:
		ADR    DATA NAME        TYPE   	LEN


   ----------------------------------------------------------------------------
	@changelog:
		V1.0: 2015-01-10: initial creation.

	@backlog:
	
	@note:
 
	@license: 
		we need to create one.

*/
#include "Arduino.h"
#include "MemoryFree.h"


//=============================================================================
/*!
	@section:  Declaration
*/
const char _version[]  = "1.0";
const char _copyright[]  = "(c) 2015 LLPA. ALL RIGHTS RESERVED";

boolean debugState = false;
#define DEBUG(x) if (debugState) Serial.println(F(x))

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16, 17, 18, 19}; // the analog 0-5 pins are also known as 14-19

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

// we will track if a button is 'currently pressed'
byte pressed[NUMBUTTONS];


//=============================================================================
/*!
	@section:  Main
	@brief
*/
void setup() {

	// set up serial port
	Serial.begin(115200);
	Serial.print("Button checker with ");
	Serial.print(NUMBUTTONS, DEC);
	Serial.println(" buttons");

	Serial.print(F("Version: "));
	Serial.println(_version);
	Serial.print(F("checkMemory()="));
	Serial.println(checkMemory());

	// pin13 LED
	pinMode(13, OUTPUT);

	// Make input & enable pull-up resistors on switch pins
	for (byte i = 0; i < NUMBUTTONS; i++) {
		pinMode(buttons[i], INPUT);
		digitalWrite(buttons[i], HIGH);
	}
}

//-----------------------------------------------------------------------------
/*!
	@brief:
	@returns:  Nothing
*/
void loop() {
	checkButtons(); // when we check the switches we'll get the current state

	for (byte i = 0; i < NUMBUTTONS; i++) {
		if (pressed[i]) {
			Serial.print(i, DEC);
			Serial.println(" pressed");
			// is the button pressed down at this moment
		}
	}
}

//-----------------------------------------------------------------------------
/*!
	@brief:
	@returns:  Nothing
*/
void checkButtons() {
	static byte previousstate[NUMBUTTONS];
	static byte currentstate[NUMBUTTONS];
	static long lasttime;
	byte i;

	// we wrapped around, lets just try again
	//if (millis()) {
	//	lasttime = millis();
	//}

	if ((lasttime + DEBOUNCE) > millis()) {
		// not enough time has passed to debounce
		return;
	}

	// ok we have waited DEBOUNCE milliseconds, lets reset the timer
	lasttime = millis();

	for (i = 0; i < NUMBUTTONS; i++) { // when we start, we clear out the "just" indicators
		currentstate[i] = digitalRead(buttons[i]);   // read the button

		if (currentstate[i] == previousstate[i]) {
			pressed[i] = !currentstate[i]; // remember, digital HIGH means NOT pressed
		}

		previousstate[i] = currentstate[i]; // keep a running tally of the buttons
	}
}
