/*
* ws2812b lib V1.0a: Raspberry Pi programming interface for controlling WS2812B RGB LEDs
*
* needs to be used with the library provided by 626Pilot from April/May 2014, named ws2812-RPi
*
* Author: Chris (Mattscheibenpost@gmail.com)
*
* January 6th, 2016 V1.0a Initial Version
*
* License: GNU GPL v2 (see License.txt)
*/

////////////////////////////////////////////////////////////////////////////ç
/// C A L L  initLEDs()  W I T H  C O R R E C T  W I R I N G  S C H E M E  //
// ======================================================================= //
// at the beginning of your program write something like:                  //
//                                                                         //
// initLEDs(matrixWidth, matrixHeight, matrixType);                        //
//                                                                         //
// for single strands (no matrix) set matrixWidth to 1                     //
// for single strands (no matrix) set matrixHeight to length of strand     //
// for single strands (no matrix) set to matrixType to 1                   //
//                                                                         //
// for a real matrix please set type according to wiring scheme chosen:    //
//                                                                         //
//          o             o             ^         o             o          //
// Type  0: ||_||¯||   1: ||/||/||   2: ||_||¯||_||   3: ||\||\||          //
//                 v             v                       v                 //
//                                                                         //
// Type  4: o=         5: o=         6:  =o           7: =o                //
//            |            /            |                \                 //
//           =             =             =               =                 //
//          |              /              |              \                 //
//           =>            =>           <=              <=                 //
//                                                                         //
//                 ^             ^      ^               ^                  //
// Type  8: ||¯||_||   9: ||\||\||  10: ||_||¯||    11: ||/||/||           //
//          o             o                    o               o           //
//                                                                         //
// Type 12:  =>       13: =>        14: <=          15: <=                 //
//          |             \               |              /                 //
//           =            =              =               =                 //
//            |           \             |                /                 //
//          o=           o=              =o              =o                //
//                                                                         //
//                                                                         //
// ( double lines symbolize strands, single lines interconnections )       //
// ( o mark positions of input to strand = Din )                           //
// ( arrows indicate strands' directions => towards Dout )                 //
// (despite schemes being illustrated with 3 or 4 elements here, they      //
//  may actually be of any length and number)                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// C A L L   endLEDs();   A T   T H E   E N D   O F   Y O U R   main()     //
/////////////////////////////////////////////////////////////////////////////

#ifndef _ws2812b_rpi_h_
#define _ws2812b_rpi_h_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include <signal.h>
#include <getopt.h>
#include "ws2812-RPi.h"
#include <time.h>
//
// use Color_t from ws2812-RPi as structure for RGB values
//
Color_t* pixel;// = array with all pixel color values, will be allocated at runtime
unsigned char brightnessLEDs; // either change with setBrightness(0...255) or by setting it directly
unsigned int idx(unsigned int row, unsigned int column); // gets index (offset) in a (linearily wired) strand
unsigned int _height; // number of rows, will be set to one for one single linear strand, will be set by calling initLEDs appropriately
unsigned int _width; // number of columns, will be set to length for one single linear strand, will be set by calling initLEDs appropriately
unsigned int _type; // ranges from 0 to 15 (see above), indicating type of wiring scheme, will be set by calling initLEDs appropriately
void initLEDs(unsigned int matrixWidthOrJustLengthForNoMatrix, unsigned int matrixHeightOrJustOneForNoMatrix, unsigned char matrixTypeAnyOfSixteenPossibleTypes); // should be called once at beginning of each program
void endLEDs(void); // should be called at end of main()-routine in order to deinitialize hardware control before returning to calling environment
void setRGB(unsigned int row, unsigned int column, unsigned long int color);
void setR(unsigned int row, unsigned int column, unsigned char red);
void setG(unsigned int row, unsigned int column, unsigned char green);
void setB(unsigned int row, unsigned int column, unsigned char blue);
unsigned long int getRGB(unsigned int row, unsigned int column);
unsigned char getR(unsigned int row, unsigned int column);
unsigned char getG(unsigned int row, unsigned int column);
unsigned char getB(unsigned int row, unsigned int column);
void setBrightness(unsigned char value); // 255 is full brightness, less will dim leds down, use 5 if in doubt
void clearLEDs(); // all LEDs off
void showLEDs();  // lights up strand with stored color value ( => displays values)
#endif
