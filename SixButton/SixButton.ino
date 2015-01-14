//=============================================================================
/*!
	@title: SixButton
	@brief:

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


//=============================================================================
/*!
	@section:  Main
	@brief:
	@returns: nothing
	@throws: nothing
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
	@returns: nothing
	@throws: nothing
*/
void loop() {
	int btn;

	if ( (btn=checkButtons()) != 0 ) {
		Serial.print(F("you pressed button number: "));
		Serial.println(btn, DEC);
	}
}

//-----------------------------------------------------------------------------
/*!
 	checkButtons
	@brief:  check all of the buttons to see if they have been pressed or not
	@returns: the first button in array presses.  0 means no button was presses.
	@throws: nothing
*/
byte checkButtons() {
	byte pstate[NUMBUTTONS];
	byte cstate[NUMBUTTONS];
	byte i;


	for (i = 0; i < NUMBUTTONS; i++) {
		pstate[i] = !digitalRead(buttons[i]);   // read the button and rev polarity because of pullup register
	}

	delay(DEBOUNCE);                            //wait DEBOUNCE time.

	for (i = 0; i < NUMBUTTONS; i++) {
		cstate[i] = !digitalRead(buttons[i]);   // read the button and rev polarity because of pullup register
	}

	for (i = 0; i < NUMBUTTONS; i++) {
		if (cstate[i] == pstate[i]) {
			if (cstate[i] == HIGH) {
				return i;
			}
		}
	}

	return 0;
}
