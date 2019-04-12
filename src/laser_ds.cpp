#include "laser_ds.h"
#include <Wire.h>
#include "VL53L1X.h"

VL53L1X sensor;
VL53L1X sensor2;
VL53L1X sensor3;
VL53L1X sensor4;


Lasers::Lasers(int F_pin, int B_pin, int L_pin, int R_pin) {
  _pinF = F_pin;
  _pinB = B_pin;
  _pinL = L_pin;
  _pinR = R_pin;
}

void Lasers::setAddy() {

  pinMode(_pinF, OUTPUT);void count1();
// void count2();
// void count3();
// void count4();
  pinMode(_pinB, OUTPUT);
  pinMode(_pinL, OUTPUT);
  pinMode(_pinR, OUTPUT);
  digitalWrite(_pinF, LOW);
  digitalWrite(_pinB, LOW);
  digitalWrite(_pinL, LOW);
  digitalWrite(_pinR, LOW);

  delay(500);
  Wire.beginTransmission(0x29);

  pinMode(_pinF, INPUT);
  delay(150);
  Serial.println("00");
  sensor.init(true);

  Serial.println("01");
  delay(100);
  sensor.setAddress((uint8_t)22);
  Serial.println("02");

  pinMode(_pinB, INPUT);
  delay(150);
  sensor2.init(true);
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)43);
  Serial.println("04");

  pinMode(_pinL, INPUT);
  delay(150);
  sensor3.init(true);
  Serial.println("05");
  delay(100);
  sensor3.setAddress((uint8_t)42);
  Serial.println("06");

  pinMode(_pinR, INPUT);
  delay(150);
  sensor4.init(true);
  Serial.println("07");
  delay(100);
  sensor4.setAddress((uint8_t)41);
  Serial.println("08");

  sensor.setTimeout(500);
  sensor2.setTimeout(500);
  sensor3.setTimeout(500);
  sensor4.setTimeout(500);

  sensor.startContinuous(50);
  sensor2.startContinuous(50);
  sensor3.startContinuous(50);
  sensor4.startContinuous(50);

  Serial.println("addresses set");
}

void Lasers::i2cScan() {
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);
    }
  }
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");

}

int Lasers::sensF() {
  int sensF = sensor.readRangeContinuousMillimeters();
  return sensF;
}

int Lasers::sensB() {
  int sensB = sensor2.readRangeContinuousMillimeters();
  return sensB;
}

int Lasers::sensL() {
  int sensL = sensor3.readRangeContinuousMillimeters();
  return sensL;
}

int Lasers::sensR() {
  int sensR = sensor4.readRangeContinuousMillimeters();
  return sensR;
}
