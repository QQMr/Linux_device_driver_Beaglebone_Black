#include "Print.h" 
#include <Wire.h>
#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#define printIIC(args)	Wire.write(args)
#else
#include "WProgram.h"

#define printIIC(args)	Wire.send(args)

#endif
#include "Wire.h"

static const uint8_t I2C_ADD = 0x27; 

extern "C" void Wrapper_Delay(uint32_t in)
{
    delay(in);
}

extern "C" void Wrapper_delayMicroseconds(uint32_t in)
{
    delayMicroseconds(in);
}

extern "C" void Wrapper_WriteOneByte_I2C(uint8_t inData)
{
   
    Wire.beginTransmission(I2C_ADD);
	printIIC(inData);
	Wire.endTransmission();   
}