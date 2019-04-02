#include "imu.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);



//Imu::Imu() {
//  //Constructor if needed in the future
//}

void Imu::start() {
  Serial.println("IMU Sensor Test");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  } else{
    Serial.println("IMU Test PASSED");
  }

  delay(100);
  bno.setExtCrystalUse(true);
}

float Imu::getPhi() {

  //Get a new sensor event
  sensors_event_t event;
  bno.getEvent(&event);

  //Get phi
  float phi = event.orientation.x;
  if(phi > 359.0 && phi < 360.0)
   {phi = 0;}
  return phi;
}

/* For more axis use :

  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

*/
