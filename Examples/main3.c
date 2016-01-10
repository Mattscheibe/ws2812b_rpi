#include "ws2812b_rpi.h"

//a matrix of LEDs changing brightness:
int main() { 
  // Init:
  initLEDs(13,10,14); // matrix with 13 rows and 10 columns, wiring starting at lower right corner, shortes wiring paths (type 14)

  unsigned int i,j;

  // all red:
  for (i=0;i<10;i++) {
    for (j=0;j<13;j++) {
      setRGB(i,j,0xFF0000); // red is FF, green is 00 and blue is 00 --> it's red
    }
  }

  // change brightness ("pulse"), repeat 10 times:
  for (j=0;j<10;j++) {
    // 1. up:
    for (i=0;i<20;i++) {
      setBrightness(i);
      showLEDs();
    }
    // 2. down
    for (i=20;i>0;i--) {
      setBrightness(i);
      showLEDs();
    }
  }
  
  // Exit cleanly:
  endLEDs(); return 0;
}
