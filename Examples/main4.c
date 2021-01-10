#include <ws2812b_rpi.h>

int main() { 
  // Init:
  initLEDsPCM(1,1,1); // 1 LED
  setMaxBrightness(3); // low brightness, won't blind you
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
    usleep(50000);
    
    // Switch LED off:
    clearLEDs();
    
    // Pause a little more:
    usleep(250000);
  }

  // Exit cleanly:
  endLEDs(); return 0;
}
