/*************************************************** 
  Septillion Shift 7seg v1.0 - Speedometer for modelrailroad
  Created by Timo Engelgeer (Septillion), May 30, 2015
  
  
****************************************************/

#ifndef SEPTILLION_SHIFT_7SEG
#define SEPTILLION_SHIFT_7SEG

#include <Arduino.h>   //Arduino IDE >= V1.0
#include "Print.h"

class Septillion_Shift_7seg{
  public:
    Septillion_Shift_7seg(uint8_t size = 8);
    
    void begin(uint8_t, uint8_t, uint8_t);
    void update();
    
    void write(uint8_t);
    void writeNum(uint8_t, bool = false);
    void writeNum(uint8_t, uint8_t, bool = false);
    void printNum(uint16_t, bool = true, uint8_t = 10);
    
    void clearDisplay();
    
    void test(uint8_t);
  
  protected:
    uint8_t _buffer[8];
    uint8_t _dataPin, _clockPin, _latchPin;
    uint8_t _dataPinMask, _clockPinMask, _latchPinMask;
    volatile uint8_t *_dataPinReg, *_clockPinReg, *_latchPinReg;
    uint8_t _size, _position, _updatePos;
    uint32_t _lastUpdate;
};

#endif