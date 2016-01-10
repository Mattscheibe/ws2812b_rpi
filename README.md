# ws2812b_rpi
A library for driving WS2812b based (aka Neopixel) LED strands connected to a Raspberry Pi. Allows for convenient setup of matrix-like displays. Uses standard addressing and standard color coding like in image processing. For code interchange, this library also exists for standard AVR or for Arduino-like boards.
- - -
* This is for a raspberry pi controlling an WS2812B LEDs strand directly from its pin GPIO18.
* Parts of this strand might be arranged in any kind of matrix shape, with any kind of wiring scheme between these parts.
* ws2812b_rpi offers image-processing-like commands and support for matrix-like connections between parts of strand.
* It is an addition/simplification to 626Pilot's gorgeous library https://github.com/626Pilot/RaspberryPi-NeoPixel-WS2812.git and must be used together with it.

- - -

## Hardware Setup
* Find reference for wiring schemes in file ws2812b.h or in WIRING.txt.
* A resistor of 220...470 ohms should be used for connecting RPi GPIO 18 (PCM_CLK) and ws2812b Din data pin.
* A capacitor of around 100uF should be connected between +5V and GND of strand. Both resistor and capacitor should be placed close to the strand.

- - -

## Installation
* copy all files from this distribution plus file ws2812-RPi.h and ws2812-RPi.c from 626Pilot's library ws2812-RPi to one
directory.
* Edit ws2812-RPi.h and ws2812-RPi.c so that settings for LED_BUFFER_LENGTH are changed to 169 (not 24 as before).

- - -

## Usage
* Type "make make" to set a program name, then type "make edit" to edit the program and type "make run" to run it.
If reluctant to typing make commands, use your favourite editor directly and register "run"-target to it (i.e., bind a key
to running command "make run").
Note that when using your code will have to start with a call of
    initLEDs(matrixWidthOrJustLengthForNoMatrix,matrixHeightOrJustOneForNoMatrix,matrixTypeAnyOfSixteenPossibleTypes);
and end with a call of
    endLEDs();

- - -

main.c and main2.c are examples for testing.

Type "make edit" and change which one is being used.
Type "make run" to start.

- - -

## Commands

```
initLEDs(width,height,type);  // attach strand to Rpi
                          // please add this at the beginning of your program
                          // for a linear strand (no matrix) set width to length of strand and height to 1
                          // for type see ws2812b_rpi.h or WIRING.txt

setR(row,column,color);   // set red value for pixel at matrix position row,column
                          // column should be zero for single linear strands (wiring type 1)
                          // color ranges from 0x00 to 0xFF

setG(row,column,color);   // dito for green value

setB(row,column,color);   // dito for blue value

setRGB(row,column,fullcolor); // set all three color values at once
                          // full color ranges from 0x000000 to 0xFFFFFF
                          // values can be taken from image processing or color picker tools

getR(row,column);         // read-out function for red channel of addressed pixel
                          // possible range from 0x00...0xFF

getG(row,column);         // dito for green channel

getB(row,column);         // dito for blue channel

getRGB(row,column);       // read-out function for full color value of addressed pixel
                          // possible range from 0x000000 to 0xFFFFFF

showLEDs();               // will display all pixels (with the colors that have been set before)

clearLEDs();              // reset all color values to 0 and display a "blank screen"

setBrightness(maxValue);  // used for dimming (good to the eyes!)
                          // maxValue ranges from 0x00 (full dimming) to 0xFF (no dimming)
                          // will usually be called once at the beginning of the program 
                          // but can also be called several times for special effects
                          // good values for maxValue while prototyping are 3...10
                          // will change intensity of display by limiting color resolution
                          // will effect all subsequent calls to showLEDs()

endLEDs();                // will release RPi's hardware, please call this at the end of your program
```
