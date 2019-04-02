//#include "i2c.h"
//#include <Wire.h>
//#include <Arduino.h>
//
//void I2C::scan() {
//  Serial.println ("I2C scanner. Scanning ...");
//  byte count = 0;
//
//  for (byte i = 1; i < 120; i++)
//  {
//
//    Wire.beginTransmission (i);
//    if (Wire.endTransmission () == 0)
//    {
//      Serial.print ("Found address: ");
//      Serial.print (i, DEC);
//      Serial.print (" (0x");
//      Serial.print (i, HEX);
//      Serial.println (")");
//      count++;
//      delay (1);
//    }
//  }
//  Serial.println ("Done.");
//  Serial.print ("Found ");
//  Serial.print (count, DEC);
//  Serial.println (" device(s).");
//}
