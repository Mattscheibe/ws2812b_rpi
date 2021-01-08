/*
* ws2812b_rpi lib: Raspberry Pi programming interface for controlling WS2812B RGB LEDs
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
// corresponding value for gpioPin above: 21                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// C A L L   endLEDs();   A T   T H E   E N D   O F   Y O U R   main()     //
/////////////////////////////////////////////////////////////////////////////

#include "ws2812b_rpi.h"

void initLEDs(unsigned int matrixWidthOrJustLengthForNoMatrix, unsigned int matrixHeightOrJustOneForNoMatrix, unsigned char matrixTypeAnyOfSixteenPossibleTypes, unsigned char gpioPin29recommendedWillImplicitlyChooseBetweenPCMorPWMorSPImode) {
  // please initially call once ( and do not forget to terminate main program by calling "endLEDs();" at its end )
  ws2811_return_t ret;
  
  unsigned int numLEDs = matrixWidthOrJustLengthForNoMatrix * matrixHeightOrJustOneForNoMatrix;
    pixel = malloc(3*numLEDs);
  _width = (unsigned int)matrixWidthOrJustLengthForNoMatrix;
  _height = (unsigned int)matrixHeightOrJustOneForNoMatrix;
  _type = matrixTypeAnyOfSixteenPossibleTypes;
  
  matrix = malloc(sizeof(ws2811_led_t) * _width * _height);
  
  ledstring.freq = TARGET_FREQ;
  ledstring.dmanum = DMA;
  
  ledstring.channel[0].gpionum = gpioPin29recommendedWillImplicitlyChooseBetweenPCMorPWMorSPImode;
  ledstring.channel[0].count = numLEDs;
  ledstring.channel[0].invert = 0;
  ledstring.channel[0].brightness = 255;
  ledstring.channel[0].strip_type = STRIP_TYPE;

  ledstring.channel[1].gpionum = 0;
  ledstring.channel[1].count = 0;
  ledstring.channel[1].invert = 0;
  ledstring.channel[1].brightness = 0;
  ledstring.channel[1].strip_type = STRIP_TYPE;

  if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
    fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
    return;
  }
  clearLEDs();
}
void initLEDsPCM(unsigned int w, unsigned int h, unsigned int t) {
  initLEDs(w,h,t,21);
}
void initLEDsPWM(unsigned int w, unsigned int h, unsigned int t) {
  initLEDs(w,h,t,12);
}
void initLEDsSPI(unsigned int w, unsigned int h, unsigned int t) {
  initLEDs(w,h,t,19);
}
void setInvertedOutput() {
  ledstring.channel[0].invert = 1;
}

unsigned int idx(unsigned int i, unsigned int j) {
  // calculates offset for chosen wiring scheme:
  unsigned int x;
  if (_type ==  0) {
    if (j%2 == 0) {
      x = j * _height + i ;
    } else {
      x = j * _height + ( _height - 1 - i ) ;
    }
  } else if (_type ==  1) {
    x = j * _height + i ;
  } else if (_type ==  2) {
    if ((_width-j)%2 == 0) {
      x = ( _width - 1 - j ) * _height + _height - 1 - i ;
    } else {
      x = ( _width - 1 - j ) * _height + i ;
    }
  } else if (_type ==  3) {
    x = ( _width - 1 - j ) * _height + i ;
  } else if (_type ==  4) {
    if (i%2 == 0) {
      x = i * _width + j ;
    } else {
      x = i * _width + ( _width - 1 - j ) ;
    }
  } else if (_type ==  5) {
    x = i * _width + j;
  } else if (_type ==  6) {
    if (i%2 == 0) {
      x = i * _width + ( _width - 1 - j ) ;
    } else {
      x = i * _width + j ;
    }
  } else if (_type ==  7) {
    x = i * _width + ( _width - 1 - j ) ;
  } else if (_type ==  8) {
    if (j%2 == 0) {
      x = j * _height + ( _height - 1 - i ) ;
    } else {
      x = j * _height + i ;
    }
  } else if (_type ==  9) {
    x = j * _height + ( _height - 1 - i ) ;
  } else if (_type == 10) {
    if ((_width-j)%2 == 0) {
      x = ( _width - 1 - j ) * _height + i ;
    } else {
      x = ( _width - 1 - j ) * _height + ( _height - 1 - i ) ;
    }
  } else if (_type == 11) {
    x = ( _width - 1 - j ) * _height + ( _height - 1 - i ) ;
  } else if (_type == 12) {
    if ((_height-i)%2 == 0) {
      x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
    } else {
      x = ( _height - 1 - i ) * _width + j ;
    }
  } else if (_type == 13) {
    x = ( _height - 1 - i ) * _width + j ;
  } else if (_type == 14) {
    if ((_height-i)%2 == 0) {
      x = ( _height - 1 - i ) * _width + j ;
    } else {
      x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
    }
  } else if (_type == 15) {
    x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
  }
  return x;
}
void setRGB(unsigned int i, unsigned int j, unsigned long int rgb) {
  unsigned char r,g,b;
  r = rgb >> 16;
  g = (rgb >> 8) & 0xFF;
  b = rgb & 0xFF;
  unsigned int x = idx(i,j);
  pixel[x].r=r;
  pixel[x].g=g;
  pixel[x].b=b;
}
void setR(unsigned int i, unsigned int j, unsigned char red) {
  unsigned int x = idx(i,j);
  pixel[x].r=red;
}
void setG(unsigned int i, unsigned int j, unsigned char green) {
  unsigned int x = idx(i,j);
  pixel[x].g=green;
}
void setB(unsigned int i, unsigned int j, unsigned char blue) {
  unsigned int x = idx(i,j);
  pixel[x].b=blue;
}
unsigned long int getRGB(unsigned int i, unsigned int j) {
  unsigned long int rgb;
  unsigned int x = idx(i,j);
  rgb = (pixel[x].r << 16) + (pixel[x].g << 8) + pixel[x].b;
  return rgb;
}
unsigned char getR(unsigned int i, unsigned int j) {
  unsigned int x = idx(i,j);
  return pixel[x].r;
}
unsigned char getG(unsigned int i, unsigned int j) {
  unsigned int x = idx(i,j);
  return pixel[x].g;
}
unsigned char getB(unsigned int i, unsigned int j) {
  unsigned int x = idx(i,j);
  return pixel[x].b;
}
void setMaxBrightness(unsigned char value) {
  ledstring.channel[0].brightness = (uint8_t)value;
}
void clearLEDs() {
  // all LEDs off
  int x;
  for (x=0;x<_width*_height;x++) {
    pixel[x].r=0;
    pixel[x].g=0;
    pixel[x].b=0;
  }
  showLEDs();
}
void showLEDs() {
  // display pixel array content to LEDs
  // a. set pixel values in transfer buffer
  unsigned int x;
  unsigned int length=_width*_height;
  for (x=0;x<length;x++) {
    ledstring.channel[0].leds[x] = (uint32_t) (pixel[x].r + pixel[x].g*256 + pixel[x].b*65536);
  }
  // b. display transfer buffer
  ws2811_render(&ledstring);
}
void endLEDs() {
  clearLEDs();
  ws2811_fini(&ledstring);
}
