/* ============================================================================
	@title: NPSKiosk
	@summary:

	@copyright: (c) 2015 LLPA. ALL RIGHTS RESERVED
	@version: 1.0

	@organization: LLPA
	@author:  Gregory L. Bodine
	@created: 2015-01-07

	@eeprom:
		ADR    DATA NAME        TYPE   	LEN
		x00    ssid			    string	30
		x1D    pw	   	    	string	30

   ----------------------------------------------------------------------------
	@changelog:
		V1.0: 2015-01-07: initial creation.

	@backlog:
		Expected Release #:
		Date of discovery:
		Description of backlog item:
		******
	
	@note:
 
	@license: 
		we need to create one.

*/
#import "EEPROM.h"
#import "SPI.h"
#import "EthernetV2_0.h"
#import "Regexp.h"
#import "YASP.h"
#import "MemoryFree.h"


/* ============================================================================
	@section:  Declaration
*/
const char _version[]  = "1.0";
const char _copyright[]  = "(c) 2015 LLPA. ALL RIGHTS RESERVED";

boolean debugState = false;


#define DEBUG(x) if (debugState) Serial.println(x)


/* ============================================================================
	@section:  EEPROM
*/
struct _eeprom {
	char ssid[30];
	char pw[30];
};
typedef struct _eeprom eeprom_t;

eeprom_t _eeprom;

void geteeprom(struct _eeprom *e){
	uint8_t i;
	byte v;
	for (i=0; i<sizeof(struct _eeprom); i++){
		v = EEPROM.read(i);
		Serial.print(v, HEX);
		if (!((i+1)%4)) Serial.print(" ");
	}

	Serial.println();
}

void seteeprom(struct _eeprom *e){
	uint8_t i;
	byte v;
	for (i=0; i<sizeof(struct _eeprom); i++){
		v = *((byte*)e+i);
		EEPROM.write(i, v);
	}

}


/* ============================================================================
	@section:  Monitor
*/

uint8_t dumpEEProm(MatchState *ms)
{
	eeprom_t e;

	geteeprom(&e);

  return YASP_STATUS_NONE;
}

uint8_t setEEProm(MatchState *ms)
{
	seteeprom(&_eeprom);

  return YASP_STATUS_NONE;
}

uint8_t debug(MatchState *ms)
{
  if (debugState)
    Serial.println("debug = on");
  else
    Serial.println("debug = off");

  return YASP_STATUS_NONE;
}

uint8_t debugOn(MatchState *ms)
{
  debugState = true;
  return YASP_STATUS_OK;
}

uint8_t debugOff(MatchState *ms)
{
  debugState = false;
  return YASP_STATUS_OK;
}

uint8_t setSSID(MatchState *ms)
{
  if (ms->level > 0)
  {

    Serial.print("sid: ");

    ms->GetCapture(_eeprom.ssid, 0);
    Serial.println(_eeprom.ssid);

    return YASP_STATUS_OK;
  }

  return YASP_STATUS_ERROR;
};

uint8_t setPW(MatchState *ms)
{
  if (ms->level > 0)
  {

    Serial.print("pw: ");
    ms->GetCapture(_eeprom.pw, 0);
    Serial.println(_eeprom.pw);

    return YASP_STATUS_OK;
  }

  return YASP_STATUS_ERROR;
};

/* ----------------------------------------------------------------------------
	note:  declare the command parser and point it to the list of commands,
	desired interface, and print function.  Any object that use the Print class
	can be used in place of the serial port (Other serial ports, including
	SoftwareSerial should all work)
*/
yaspCommand_t cmds[] =
{
  { "^debug$", "debug : Return current debug state", debug },
  { "^debug on$", "debug on : Set debug on", debugOn },
  { "^debug off$", "debug off : Set debug off", debugOff },
  { "^ssid%s+(.*)$", "sid <string>: Set SID for Wifi", setSSID },
  { "^pw%s+(.*)$", "pw <string>: Set PW for Wifi", setPW },
  { "^de$", "de: dump eeprom", dumpEEProm },
  { "^se$", "se: set eeprom", setEEProm },
  //{ "^info$", "info: display information", dumpInfo },
  { 0, 0, 0 } // you must terminate this list with nulls.
};

yasp cmdParser(&cmds[0], &Serial, &Print::write);


/* ============================================================================
	@section:  Main
*/
void setup()
{
  // Initialize the serial port.
  Serial.begin(115200);

  Serial.print("Version: "); Serial.println(_version );
  Serial.print("checkMemory()="); Serial.println(checkMemory());


}

void loop()
{
  if (Serial.available()){
    // Pass received data to the command parser.  If a complete command is received
    // the callback will be activated from this call.
    cmdParser.addData(Serial.read());
  }
}
