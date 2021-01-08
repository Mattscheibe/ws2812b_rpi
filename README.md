# ws2812b_rpi
A library for driving a single WS2812b based (aka Neopixel) LED strand connected to a Raspberry Pi. Allows for convenient setup of matrix-like displays. Uses standard addressing and standard color coding like in image processing. For code interchange, this library also exists for standard AVR (https://github.com/Mattscheibe/ws2812b.git) or for Arduino-like boards (https://github.com/Mattscheibe/WS2812b_arduino.git, supports more than one strand).
Maximum number of pixels is 5400. Strand can be cut and glued to a matrix shape. Any kind of arrangement is supported (wiring across matrix, wiring at edgeds, first DIN on any of four corners of matrix rectangle).
- - -
* This is for a raspberry pi controlling an WS2812B LEDs strand directly from its pin GPIO21 (PCM mode) oder GPIO18 (PWM mode) oder GPIO12 (SPI mode).
* Parts of this strand might be arranged in any kind of matrix shape, with any kind of wiring scheme between these parts.
* ws2812b_rpi offers image-processing-like commands and support for matrix-like connections between parts of strand.
* It is an "addition/simplification for C" to J. Graff's gorgeous library rpi_ws281x https://github.com/jgarff/rpi_ws281x and must be used together with it.
* Read J. Graff's introduction to different modes: some RPIs need special settings for PWM mode or for SPI mode. PCM mode will usually work "out of the box".

- - -

## Hardware Setup
* Find reference for wiring schemes in file ws2812b_rpi.h or in WIRING.txt.
* A resistor of 220...470 ohms should be used for connecting RPi GPIO 21 (PCM_DOUT) and ws2812b Din data pin.
* A capacitor of around 100uF should be connected between +5V and GND of strand. Both resistor and capacitor should be placed close to the strand.
* A diode like 1N4007 (or any other 1A type) should be used to get RPi's +5V to 5V of strand (thus reducing voltage, so that RPi's signal will have an appropriate level for the strand.

- - -

## Installation
* download and install J. Graff's rpi_directory (might need cmake for doing this)
* type "sudo make install" to install this library
* Copy over Makefile from examples to have a starting point for your projects. Edit Makefile to change EDITOR to your favourite editor.

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

main.c and main2.c are examples for testing. A Makefile accompanies these examples. Copy it and use it for your own code.
You might first edit Makefile and change target and file names and also your editor's name before you start your project.
Afterwards
Type "make edit" and enter your code.
Type "make run" to start your code.
Type CTRL-\ to stop, or, if using STOPPABLE macro for your main loop, simply type CTRL-C to stop.

- - -

## Commands

```
initLEDs(width,height,type,gpio); // attach strand to Rpi
                                  // please add this at the beginning of your program
                                  // for a linear strand (no matrix) set width to length of strand and height to 1
                                  // for type see ws2812b_rpi.h or WIRING.txt
                                  // preferred gpio number should be 21 for PCM mode

initLEDsPCM(width,height,type);   // shortcut for choosing gpio number 21 (PCM mode) -- usually this is the best choice
initLEDsPWM(width,height,type);   // shortcut for choosing gpio number 18 (PWM mode) -- RPi PWM audio should be deactivated
initLEDsSPI(widht,height,type);   // showtcut for choosing gpio number 12 (SPI mode) -- RPi core speed should be set to a fixed value on Pi 3 and above

setR(row,column,color);   // set red value for pixel at matrix position row,column
                          // column should be zero for single linear strands (wiring type 1)
                          // color ranges from 0x00 to 0xFF

setG(row,column,color);   // dito for green value

setB(row,column,color);   // dito for blue value

setRGB(row,column,fullcolor); // set all three color values at once
                          // full color ranges from 0x000000 to 0xFFFFFF
                          // values can be taken from image processing or color picker tools
                          // byte order is RRGGBB, e.g. 0xFACE00 means red is FF,green AA, blue is 00 (which adds to a reddish yellow)

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

endLEDs();                // will release RPi's hardware, please call this at the end of your program, especially if in PWM or in SPI mode
```
