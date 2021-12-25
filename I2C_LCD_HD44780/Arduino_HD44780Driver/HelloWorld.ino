//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
// #include <LiquidCrystal_I2C.h>
#include "HD44780U_I2C_Burton.h"

// LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Wire.begin();
  LCD_init();                      // initialize the lcd 
  LCD_init();
  // Print a message to the LCD.
  backlight();
  setCursor(3,0);
  WriteMessage("Hello, world!");
  setCursor(2,1);
  WriteMessage("Ywrobot Arduino!");
  setCursor(0,2);
  WriteMessage("Arduino LCM IIC 2004");
  setCursor(2,3);
  WriteMessage("Power By Ec-yuan!");
  setCursor(0,0);
}


void loop()
{
  delay(1000);
  scrollDisplayRight();
}
