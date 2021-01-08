#include <ws2812b_rpi.h>
int main() {
  initLEDsPCM(1,1,1); // connect DIN to GPIO21 (pin 40), single pixel (width 1, height 1, so 1 pixel all together...)
  setMaxBrightness(5); // dim down, avoid overstraining Pi and eye
  // use macro for ctrl-c stoppable main loop, alternative to "while (1) {...}"
  STOPPABLE {
    setR(0,0,0xFF); // only sets red pixel to full (but dimmed to lower value, see above)
    showLEDs();     // update "screen"
    usleep(200000); // sleep for 200000 micro seconds = 200 milli seconds = 0.2 seconds
    setR(0,0,0x00); // set red intensitiy to zero (might have used clearLEDs() instead)
    showLEDs();     // update "screen"
    sleep(1);				// wait for 1 second
  }
  endLEDs(); // switch off LEDs and free interfaces (would be more important when using PWM mode or SPI mode)
  return 0;  // so very happy now ...
}
  