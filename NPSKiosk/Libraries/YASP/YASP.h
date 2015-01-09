/**
 * Copyright (C) <2012> <Mike Dellisanti>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef __YASP_H
#define __YASP_H

#include <Arduino.h>
#include <Regexp.h>

// YASP VERSION ALPHA 1

// If the user has defined a custom length for the command
// buffer, set it here.
// TODO: this isn't really functional yet.  I think that I
// need to allocate the buffer at runtime since the library
// appears to be built before the user code.
#if defined(MAX_YASP_CMD_LEN)
  #define MAX_CMD_LEN MAX_YASP_CMD_LEN
#else
  #define MAX_CMD_LEN 64
#endif

#define CP_LIB_VERSION 100 // library version X.YY (e.g. 1.00) 100 means 1.00

// User callbacks should use one of these return codes.
enum
{
  YASP_STATUS_NONE  = 0,    // Don't print anything
  YASP_STATUS_OK        = 1,    // Print 'OK'
  YASP_STATUS_ERROR     = 2 // Print error message
};

// This is the format of the command table for parsing serial commands
typedef struct {
  const char *regexp;
  const char *help;
  uint8_t (*callback)(MatchState *ms);
} yaspCommand_t;


typedef size_t (Print::*yaspPrintCallback_t)(const char *);

class yasp {


public:
    const char*     errMsg;
    const char*     userPrompt;
    const char*     EOL;
    const char*     helpCmd;

    // library version
    uint8_t LibVer(void);

    yasp(yaspCommand_t *cmdTable, Print* printObj, yaspPrintCallback_t printCB);
    ~yasp();

    void addData(byte data);
    void parseCommand(char* command);

private:
    yaspCommand_t*  mCmdTable;
    Print*          mPrint;
    yaspPrintCallback_t   mPrintCB;
    void printResponse(const char *, boolean);
};

#endif // __YASP_H
