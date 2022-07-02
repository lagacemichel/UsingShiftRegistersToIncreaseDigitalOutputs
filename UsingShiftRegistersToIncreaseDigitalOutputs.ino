/*
  Display gray scale counter on 10 LEDs using latched shift registers

  Program that displays Gray scale values on 10 LEDs through a latched
  shift register, allowing to drive 10 LEDs using three digital outputs
  instead of ten. It is associated with the "Using Shift Registers to
  Increase Digital Outputs" blog post at https://lagacemichel.com

  MIT License

  Copyright (c) 2022, Michel Lagace

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "LatchedShiftRegisterChannel.h"

// Create bar graph display instance including shift register control pins
const int SERIAL_DATA_DIO = 2;         // Serial data input  - SN74HC595 pin 14
const int SERIAL_CLOCK_DIO = 3;        // Serial data clock  - SN74HC595 pin 11
const int LATCH_SIGNAL_DIO = 4;        // Output latch clock - SN74HC595 pin 12
const int NUMBER_OF_LEDS = 10;         // Number of LEDs to drive
const int NUMBER_OF_DIGITAL_OUTPUTS = 16; // Number of available shift register digital outputs
const int NUMBER_OF_BYTES = NUMBER_OF_DIGITAL_OUTPUTS/8;
const SerialCommunicationPins COMMUNICATION_PINS = {SERIAL_CLOCK_DIO, SERIAL_DATA_DIO, LATCH_SIGNAL_DIO};
const int WAIT_TIME = 100;             // 100 milliseconds per increment to get a one second full cycle
const int LARGEST_VALUE = (1 << NUMBER_OF_LEDS) - 1;
LatchedShiftRegisterChannel Leds(COMMUNICATION_PINS, NUMBER_OF_DIGITAL_OUTPUTS);
int displayValue = 1;
byte outputByteArray[NUMBER_OF_BYTES];

void setup() {
  Leds.setNumberOfActiveDigitalOutputs(NUMBER_OF_LEDS);
}

void loop() {
  outputValueToLeds();
  shiftValueLeft();
  delay(WAIT_TIME);
}

void outputValueToLeds()
{
  moveIntegerIntoOutputByteArray(displayValue);
  Leds.transmit(outputByteArray, NUMBER_OF_LEDS);
}

void moveIntegerIntoOutputByteArray(int integer)
{
  for (int i = 0; i < NUMBER_OF_BYTES; i++)
  {
    outputByteArray[i] = integer & 0x0ff;
    integer >>= 8;
  }
}

void shiftValueLeft()
{
  displayValue = displayValue << 1;
  if (displayValue > LARGEST_VALUE)
  {
    displayValue = 1;
  }
}
