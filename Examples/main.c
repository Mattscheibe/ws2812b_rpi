#include "ws2812b_rpi.h"

int main() { 
  // Init:
  initLEDs(1,9,1); // 1 linear strand with 9 leds, therefore connection type 1
  setMaxBrightness(3); // low brightness, won't blind you

  // Nice color pattern:
  int x;
  for (x=0;x<9;x++) {
    setR(x,0,31*x);
    setG(x,0,255-31*x);
    setB(x,0,0);
  }

  // Display it:
  showLEDs();

  // Exit cleanly:
  endLEDs(); return 0;
}
