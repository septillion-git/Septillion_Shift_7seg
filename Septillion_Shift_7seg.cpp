/*************************************************** 
  SpeedOmeter v1.1 - Speedometer for modelrailroad
  Created by Timo Engelgeer (Septillion), May 30, 2015
  Based on Speedometer bij Max Roodveldt
  
  Calculates the (scale) speed of a (model) train based on two 
  detections on the layout. This can be any kind you like.
  
****************************************************/
#include "Septillion_Shift_7seg.h"

static const uint8_t numbertable[] = {
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};

Septillion_Shift_7seg::Septillion_Shift_7seg(uint8_t size){
  _size = size;
  _position = 0;
}

void Septillion_Shift_7seg::begin(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin){
  uint8_t dPort, cPort, lPort;
  
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;
  
  dPort = digitalPinToPort(dataPin);
  cPort = digitalPinToPort(clockPin);
  lPort = digitalPinToPort(latchPin);
  
  if(dPort == NOT_A_PIN || cPort == NOT_A_PIN || lPort == NOT_A_PIN) return;
  
  _dataPinReg = portOutputRegister(dPort);
  _clockPinReg = portOutputRegister(cPort);
  _latchPinReg = portOutputRegister(lPort);
  
  _dataPinMask = digitalPinToBitMask(dataPin);
  _clockPinMask = digitalPinToBitMask(clockPin);
  _latchPinMask = digitalPinToBitMask(latchPin);
  
  pinMode(dataPin,  OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  
  digitalWrite(latchPin, LOW);
  this->clearDisplay();
}

void Septillion_Shift_7seg::update(){
  //if(millis() - _lastUpdate > 1){
    _lastUpdate = millis();
    digitalWrite(_latchPin, LOW);
    shiftOut(_dataPin, _clockPin, MSBFIRST, _buffer[_updatePos]);
    shiftOut(_dataPin, _clockPin, MSBFIRST, 1 << _updatePos);
    digitalWrite(_latchPin, HIGH);
    
    _updatePos++;
    if(_updatePos == _size){
      _updatePos = 0;
    }
  //}
}
    

void Septillion_Shift_7seg::writeNum(uint8_t num, bool decimal_point){
  this->writeNum(num, _position++, decimal_point);
}

void Septillion_Shift_7seg::writeNum(uint8_t num, uint8_t pos, bool decimal_point){
  if(pos < _size && num < 16){
    _buffer[pos] = ~(numbertable[num] | (decimal_point ? 0x80: 0x00));
  }
}

void Septillion_Shift_7seg::printNum(uint16_t num, bool right, uint8_t base){
  if(base > 16){
    base = 10;
  }
  
  if(right){
    for(_position = 0; _position < _size && num; _position++){
      this->writeNum(num % base, _position);
      num /= base;
    }
    while(_position < _size) _buffer[_position++] = 0xFF;
  }
}

void Septillion_Shift_7seg::clearDisplay(){
  for(byte i = 0; i < _size; i++){
    _buffer[i] = 0xFF;
  }
}

void Septillion_Shift_7seg::test(uint8_t val){
  uint8_t oldSREG = SREG;
	cli();

	if (val == LOW) {
		*_dataPinReg &= ~_dataPinMask;
	} else {
		*_dataPinReg |= _dataPinMask;
	}

	SREG = oldSREG;
}