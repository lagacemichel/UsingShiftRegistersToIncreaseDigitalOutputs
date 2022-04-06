/*
  Latched Shift Register Channel Class (Implementation)

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

#include "LatchedShiftRegisterChannel.h"

const int BITS_IN_BYTE = 8;

inline int clampValue(int value, int minimumValue, int maximumValue)
{
  int clampedValue = value;
  if (clampedValue > maximumValue)
  {
    clampedValue = maximumValue;
  }
  if (clampedValue < minimumValue)
  {
    clampedValue = minimumValue;
  }
  return clampedValue;
}

LatchedShiftRegisterChannel::~LatchedShiftRegisterChannel()
{
  clearShiftRegister();
  makeCommunicationPinsHighImpedance();
}

LatchedShiftRegisterChannel::LatchedShiftRegisterChannel(SerialCommunicationPins pins, int numberOfShiftRegisterBits)
{
  communicationPins = pins;
  totalNumberOfDigitalOutputs = clampValue(numberOfShiftRegisterBits, 0, MAXIMUM_SERIAL_BITS);
  numberOfActiveDigitalOutputs = totalNumberOfDigitalOutputs;
  prepareCommunicationPins();
  clearShiftRegister();
}

void LatchedShiftRegisterChannel::transmit(const byte transmissionBuffer[], int transmissionSizeInBits) const
{
  int bitsToSend = clampValue(transmissionSizeInBits, 0, numberOfActiveDigitalOutputs);
  sendBitsOneByteAtATime(transmissionBuffer, bitsToSend);
  int paddingBitsToSend = numberOfActiveDigitalOutputs - bitsToSend;
  sendPaddingBits(paddingBitsToSend);
  latch();
}

void LatchedShiftRegisterChannel::setNumberOfActiveDigitalOutputs(int numberOfActiveShiftRegisterBits)
{
  numberOfActiveDigitalOutputs = clampValue(numberOfActiveShiftRegisterBits, 0, totalNumberOfDigitalOutputs);
}

void LatchedShiftRegisterChannel::prepareCommunicationPins() const
{
  pinMode(communicationPins.serialClockPin, OUTPUT);
  pinMode(communicationPins.serialDataPin, OUTPUT);
  pinMode(communicationPins.latchClockPin, OUTPUT);
  digitalWrite(communicationPins.serialClockPin, LOW);
  digitalWrite(communicationPins.serialDataPin, LOW);
  digitalWrite(communicationPins.latchClockPin, LOW);
}

void LatchedShiftRegisterChannel::makeCommunicationPinsHighImpedance() const
{
  pinMode(communicationPins.serialClockPin, INPUT);
  pinMode(communicationPins.serialDataPin, INPUT);
  pinMode(communicationPins.latchClockPin, INPUT);
}

void LatchedShiftRegisterChannel::clearShiftRegister() const
{
  for (int i = 0; i < totalNumberOfDigitalOutputs; i++)
  {
    sendASingleBit(LOW);
  }
  latch();
}

void LatchedShiftRegisterChannel::sendBitsOneByteAtATime(const byte transmissionBuffer[], int numberOfBitsToTransmit) const
{
  int bytesToSend = numberOfBitsToTransmit / BITS_IN_BYTE;
  for (int byteIndex = 0; byteIndex < bytesToSend; byteIndex++)
  {
    sendBitsFromOneByte(transmissionBuffer[byteIndex], BITS_IN_BYTE);
  }
  int bitsLeft = numberOfBitsToTransmit % BITS_IN_BYTE;
  if (bitsLeft > 0)
  {
    sendBitsFromOneByte(transmissionBuffer[bytesToSend], bitsLeft);
  }
}

void LatchedShiftRegisterChannel::sendBitsFromOneByte(byte transmissionByte, int numberOfBitsToTransmit) const
{
  byte byteToSend = transmissionByte;
  int bitsToSend = clampValue(numberOfBitsToTransmit, 0, BITS_IN_BYTE);
  for (int bitIndex = 0; bitIndex < bitsToSend; bitIndex++)
  {
    sendASingleBit((byteToSend & 1) == 1);
    byteToSend >>= 1;
  }
}

void LatchedShiftRegisterChannel::sendPaddingBits(int numberOfBitsToTransmit) const
{
  for (int i = 0; i < numberOfBitsToTransmit; i++)
  {
    sendASingleBit(LOW);
  }
}

void LatchedShiftRegisterChannel::sendASingleBit(bool digitalOutputValue) const
{
  digitalWrite(communicationPins.serialDataPin, digitalOutputValue);
  digitalWrite(communicationPins.serialClockPin, HIGH);
  digitalWrite(communicationPins.serialClockPin, LOW);
}

void LatchedShiftRegisterChannel::latch() const
{
  digitalWrite(communicationPins.latchClockPin, HIGH);
  digitalWrite(communicationPins.latchClockPin, LOW);
}
