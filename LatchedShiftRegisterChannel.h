/*
  Latched Shift Register Channel Class (Header)

  The LatchedShiftRegisterChannel class allows the user to serially transmit
  data to a string of latched shift registers through three digital output
  pins: serial data, serial clock and latch. This class is used as part of
  the "Driving Seven-Segment Displays with Shift Registers" post at
  https://lagacemichel.com

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

#if !defined(LATCHEDSHIFTREGISTERCHANNEL_HEADER)
#define LATCHEDSHIFTREGISTERCHANNEL_HEADER
#include "Arduino.h"

const int MAXIMUM_SERIAL_BITS = 1024;

struct SerialCommunicationPins {
  int serialClockPin;
  int serialDataPin;
  int latchClockPin;
};
 
class LatchedShiftRegisterChannel {
  public:
    ~LatchedShiftRegisterChannel(); // Destructor
    LatchedShiftRegisterChannel(SerialCommunicationPins pins, int numberOfShiftRegisterBits);
    void transmit(const byte transmissionBuffer[], int transmissionSizeInBits) const;
    void setNumberOfActiveDigitalOutputs(int numberOfActiveShiftRegisterBits);
  private:
    LatchedShiftRegisterChannel();
    LatchedShiftRegisterChannel(const LatchedShiftRegisterChannel&);
    LatchedShiftRegisterChannel& operator = (const LatchedShiftRegisterChannel&);
    void sendASingleBit(bool digitalOutputValue) const;
    void latch() const;
    void clearShiftRegister() const;
    void prepareCommunicationPins() const;
    void makeCommunicationPinsHighImpedance() const;
    void sendPaddingBits(int numberOfBitsToTransmit) const;
    void sendBitsOneByteAtATime(const byte transmissionBuffer[], int numberOfBitsToTransmit) const;
    void sendBitsFromOneByte(byte transmissionByte, int numberOfBitsToTransmit) const;

  private:
    SerialCommunicationPins communicationPins;
    int numberOfActiveDigitalOutputs;
    int totalNumberOfDigitalOutputs;
};

#endif
