/*
* ws2812b lib: Raspberry Pi programming interface for controlling WS2812B RGB LEDs
*
* needs to be used with the library provided by Jeremy Garff, named rpi_ws281x
*
* note that former versions of this interface (before Jan 6th, 2021) used a different
* library, from 626pilot, that only worked well on RPi 1, and not on any newer models
*
* Author: Chris (Mattscheibenpost@gmail.com)
*
* January 6th, 2021 V1.1a
*
* License: GNU GPL v2 (see LICENSE file)
*/

////////////////////////////////////////////////////////////////////////////ç
/// C A L L  initLEDs()  W I T H  C O R R E C T  W I R I N G  S C H E M E  //
// ======================================================================= //
// at the beginning of your program write something like:                  //
//                                                                         //
// initLEDs(matrixWidth, matrixHeight, matrixType, gpioPin);               //
//                                                                         //
// for single strands (no matrix) set matrixWidth to 1                     //
// for single strands (no matrix) set matrixHeight to length of strand     //
// for single strands (no matrix) set matrixType to 1                      //
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
// for gpioPin choose any of those suggested by Jeremy Garff's library     //
// - and make sure that this library is properly installed first, else     //
// this interface's execution will fail                                    //
//                                                                         //
// I recommend using pin 40 (GPIO29 = PCM_DOUT) because this will activate //
// PCM mode which is compatible with any Pi version and does not implicate //
// that you either (disable PWM-audio) or (enable SPI and force core speed //
// to be fixed) for newer Pi models. If you absolutely want to use PWM or  //
// SPI, change Pi's settings beforehand and preferably use one of pin 12   //
// (GPIO18 = PWM0), for PWM mode, or pin 19 (GPIO12 = SPI_MOSI), for       //
//  maximum code compatibility between different Pi versions.              //
//                                                                         //
// Recommended wiring scheme:   Pi                           LED           //
// (for short strands that      ==                           ===           //
//  can be powered from Pi's    pin 2 +5V-----------|>|------5V            //
//  +5V, max. 1A current)                          1N4007                  //
//                                                                         //
//                              pin 40 PCM_DOUT----/\/\/\----DIN           //
//                                                  470R                   //
//                                                                         //
//                              pin 20 GND-------------------GND           //
//                                                                         //     
// corresponding value for gpioPin above: 29                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// C A L L   endLEDs();   A T   T H E   E N D   O F   Y O U R   main()     //
/////////////////////////////////////////////////////////////////////////////
#ifndef _ws2812b_rpi_h_
#define _ws2812b_rpi_h_
#include <ws2811.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

// some settings that should be okay to use with WS2812b                   //
#define DMA	10
#define STRIP_TYPE WS2811_STRIP_GBR
#define TARGET_FREQ WS2811_TARGET_FREQ

//
// use RGB as structure for color values :
//
typedef struct Color_t {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Color_t;
Color_t* pixel;// array with all pixel color values, will be allocated at runtime

ws2811_led_t *matrix;// color values as of ws2811-library
ws2811_t ledstring; // all infos about (exactly) one strand of led pixels attached to Pi's GPIO pin
static volatile sig_atomic_t running = 1; // might be used in main() program if it (or its main loop) is supposed to be stoppable (by pressing CTRL-C)
static void sig_handler(int _) {
    (void)_;
    running = 0;
}
#define STOPPABLE signal(SIGINT, sig_handler);while(running)
unsigned int numLEDs; // number of led pixels in (this) strand
unsigned int _height; // number of rows, will be set to one for one single linear strand, will be set by appropriately calling initLEDs()
unsigned int _width; // number of columns, will be set to length for one single linear strand, will be set by appropriately calling initLEDs()
unsigned int _type; // ranges from 0 to 15 (see above), indicating type of wiring scheme, will be set by calling appropriately initLEDs()

unsigned int idx(unsigned int row, unsigned int column); // gets index (offset) in a (linearily wired) strand
void initLEDs(unsigned int matrixWidthOrJustLengthForNoMatrix, unsigned int matrixHeightOrJustOneForNoMatrix, unsigned char matrixTypeAnyOfSixteenPossibleTypes, unsigned char gpioPin29recommendedWillImplicitlyChooseBetweenPCMorPWMorSPImode); // should be called once at beginning of each program
void initLEDsPCM(unsigned int w, unsigned int h, unsigned int t); // dito
void initLEDsPWM(unsigned int w, unsigned int h, unsigned int t); // standard version for PWM, uses GPIO18 ( pin 12 ); switch off PWM audio on Pi before using
void initLEDsSPI(unsigned int w, unsigned int h, unsigned int t); // standard version for SPI, uses GPIO12 ( pin 19 ); set fixed core speed on newer Pis before using
void endLEDs(); // should be called at end of main()-routine in order to deinitialize hardware control before returning to calling environment, may be omitted for PCM mode only
void setRGB(unsigned int row, unsigned int column, unsigned long int color);
void setR(unsigned int row, unsigned int column, unsigned char red);
void setG(unsigned int row, unsigned int column, unsigned char green);
void setB(unsigned int row, unsigned int column, unsigned char blue);
void setInvertedOutput(); // if GPIO output needs to be inverted (e.g. if connected via transistor or via any other inverting buffer), call this once before sending any data with showLEDs() or clearLEDs()
unsigned long int getRGB(unsigned int row, unsigned int column);
unsigned char getR(unsigned int row, unsigned int column);
unsigned char getG(unsigned int row, unsigned int column);
unsigned char getB(unsigned int row, unsigned int column);
void setMaxBrightness(unsigned char value); // 255 is full brightness (default value), less will dim leds down, use 5 if in doubt
void clearLEDs(); // all LED pixels off
void showLEDs();  // lights up strand with stored color values ( => displays values formerly set with setRGB() etc.), call this repeatedly in your main()'s loop
#endif
