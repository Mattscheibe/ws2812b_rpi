#include "ws2812b_rpi.h"
#include <wiringPi.h>

int main() { 
  // Init:
  initLEDs(1,1,1); // 1 LED
  setBrightness(3); // low brightness, won't blind you
  // start WiringPi:
  if (wiringPiSetupSys() == -1)
    return 1;

  // run for a while
  unsigned char x;
  for (x=0; x < 255; x++) {
    
    // Set LED to "red":
    setR(0,0,255);

    // Display it:
    showLEDs();
    
    // Pause a little:
    delay(50);
    
    // Switch LED off:
    clearLEDs();
    
    // Pause a little more:
    delay(250);
  }

  // Exit cleanly:
  endLEDs(); return 0;
}
