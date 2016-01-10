/*
* ws2812b_rpi lib V1.0a: Raspberry Pi programming interface for controlling WS2812B RGB LEDs
*
* needs to be used with the library provided by 626Pilot from April/May 2014, named ws2812-RPi
*
* Author: Chris (Mattscheibenpost@gmail.com)
*
* January 9th, 2016 V1.0a Initial Version
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

#include "ws2812b_rpi.h"
void initLEDs(unsigned int matrixWidthOrJustLengthForNoMatrix, unsigned int matrixHeightOrJustOneForNoMatrix, unsigned char matrixTypeAnyOfSixteenPossibleTypes) {
  // please initially call once ( and do not forget to terminate main program by calling "endLEDs();" at its end )
 
  // Check "Single Instance"
  int pid_file = open("/var/run/whatever.pid", O_CREAT | O_RDWR, 0666);
  int rc = flock(pid_file, LOCK_EX | LOCK_NB);
  if(rc) {
      if(EWOULDBLOCK == errno)
      {
          // another instance is running
          printf("Instance already running\n");
          exit(EXIT_FAILURE);
      }
  }

  // Catch all signals possible - it's vital we kill the DMA engine on process exit!
  int i;
  for (i = 0; i < 64; i++) {
          struct sigaction sa;
          memset(&sa, 0, sizeof(sa));
          sa.sa_handler = terminate;
          sigaction(i, &sa, NULL);
  }

  // Don't buffer console output
  setvbuf(stdout, NULL, _IONBF, 0);

  // How many LEDs (also, define accordingly in ws2812-RPi.h)
  numLEDs = 169;
  
  // Init PWM generator and clear LED buffer
  initHardware();
  clearLEDBuffer();

  // brightness will be handled elsewhere, so:
  setRPiBrightness(1.0);
  
  // initialize our own pixel and brightness handling and clear leds at startup
  pixel = (Color_t*)malloc(matrixWidthOrJustLengthForNoMatrix*matrixHeightOrJustOneForNoMatrix*3);
  _width = matrixWidthOrJustLengthForNoMatrix;
  _height = matrixHeightOrJustOneForNoMatrix;
  _type = matrixTypeAnyOfSixteenPossibleTypes;
  brightnessLEDs=255;
  clearLEDs();

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
void setBrightness(unsigned char value) {
  brightnessLEDs = value;
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
  if (brightnessLEDs == 255) {
    for (x=0;x<length;x++) {
      setPixelColorT(x,pixel[x]);
    }
  } else {
    float currentBrightness = (float)brightnessLEDs / 255.0 ;
    for (x=0;x<length;x++) {
      setPixelColor(x,(unsigned char)((float)pixel[x].r*currentBrightness),(unsigned char)((float)pixel[x].g*currentBrightness),(unsigned char)((float)pixel[x].b*currentBrightness));
    }
  }
  // b. display transfer buffer
  show();
}
void endLEDs() {
  terminate(0);
}
