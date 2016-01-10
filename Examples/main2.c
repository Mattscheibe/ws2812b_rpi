#include "ws2812b_rpi.h"

// some color effects on a matrix of LEDs:
int main() { 
  // Init:
  initLEDs(13,10,14); // matrix with 13 rows and 10 columns, wiring starting at lower right corner, shortes wiring paths (type 14)
  setBrightness(1); // low brightness, won't blind you

  // step by step to green
  unsigned int i,j;
  for (i=0;i<10;i++) {
    for (j=0;j<13;j++) {
      setG(i,j,0xFF); // green
      showLEDs();
    }
  }
  // and switch to blue:
  clearLEDs();
  for (i=0;i<10;i++) {
    for (j=0;j<13;j++) {
      setB(i,j,0xFF); // blue
    }
  }
  // show this often, to pause a little:
  for (i=0;i<150;i++) {
    showLEDs();
  }
  // and switch to red:
  for (i=0;i<10;i++) {
    for (j=0;j<13;j++) {
      setRGB(i,j,0xFF0000); // red is FF, green is 00 and blue is 00 --> it's red
    }
  }
  showLEDs();

  // Exit cleanly:
  endLEDs(); return 0;
}
