//#include "Arduino.h"
//#include "robot.h"
//#include "Imu.h"
//
//robot::robot() {}
//
//robot::robot(Motor m1, Motor m2, Motor m3, Motor m4, int Max, Imu imuSensor ) {
//
//  _M1 = m1;
//  _M2 = m2;
//  _M3 = m3;
//  _M4 = m4;
//
//
//  if (Max >= 0 && Max <= 255) {
//    maxSpeed = Max;
//  } else {
//    maxSpeed = 0;
//  }
//
//
//}
//
//void robot::align(int theta) {
//
//  int navCase;
//
//  int a = theta - 23;
//  float phi = imuSensor.getPhi();
//  Serial.print("a =");
//  Serial.println(a);
//  Serial.print("phi =");
//  Serial.println(phi);
//
//
//
//  if (theta <= 180) { //theta <= 180
//    if ((phi <= theta + 180) && (phi > theta)) {    //phi is b/t theta and (theta + 180)
//      Serial.println("This is really bad ------------------------------------------------------------------------>");
//      navCase = 0;
//
//      //      phi = imuSensor.getPhi();
//      //      while (phi - a < .1) {
//      //        rotateRight(100);
//      //        phi = imuSensor.getPhi();
//      //      }
//      //      stopp();
//      //      return;
//    } 
//    
//    else if ((phi < theta)) {  //theta > phi >= 0
//
//      navCase = 1;
//      //                  while (phi - a < .1) {
//      //        rotateLeft(100);
//      //        phi = imuSensor.getPhi();
//    }
//
//   else if (phi > theta + 180) { // theta + 180  < phi < 360
//
//    navCase = 2;
//
//    //      float b = abs(phi - theta);
//    //      while (b > 5) {
//    //        rotateLeft(100);
//    //        phi = imuSensor.getPhi();
//    //        b = abs(phi - a);
//    //      }
//
//    //    stopp();
//    //    return;
//  }
//  }
//  else {    //theta > 180
//    if ((phi > theta - 180) && (phi < theta)) {   //phi < a but > (a-180)
//      navCase = 3;
//      //      while (phi - a < .1) {
//      //        rotateLeft(100);
//      //        phi = imuSensor.getPhi();
//      //
//      //      }
//      //      stopp();
//      //      return;
//    } else if (phi > theta) { //phi > a but < (a-180)
//      navCase = 4;
//
//      while (phi - a > .1) {
//        rotateRight(100);
//        phi = imuSensor.getPhi();
//
//      }
//      stopp();
//      return;
//    } else if (phi <= theta - 180) {
//      //        while (abs(phi - a) > 5) {
//      //        rotateRight(100);
//      //        phi = imuSensor.getPhi();
//      //      }
//      //      stopp();
//    }
//  }
//  stopp();
//  Serial.println("test 6 stopped. exit align");
//
//
//  switch (navCase) {
//
//    case 0:
//
//      Serial.println("case = 0");
//      Serial.print("phi = ");
//      Serial.println(imuSensor.getPhi());
//      Serial.print("theta = ");
//      Serial.println(theta);
//      while (phi - a < .1) {
//        rotateRight(100);
//        phi = imuSensor.getPhi();
//      }
//      stopp();
//      break;
//
//    case 1:
//
//      Serial.println("case = 1");
//      Serial.print("phi");
//      Serial.println(imuSensor.getPhi());
//      Serial.print("theta = ");
//      Serial.println(theta);
//      while (phi - a < .1) {
//        rotateLeft(100);
//        phi = imuSensor.getPhi();
//      }
//      stopp();
//      break;
//
//    case 2:
//
//      Serial.println("case = 2");
//      Serial.print("phi");
//      Serial.println(imuSensor.getPhi());
//      Serial.print("theta = ");
//      Serial.println(theta);
//      while (abs(phi - theta) > 5) {
//        rotateLeft(100);
//        phi = imuSensor.getPhi();
//      }
//      stopp();
//      break;
//
//    case 3:
//
//      Serial.println("case = 3");
//      Serial.print("phi");
//      Serial.println(imuSensor.getPhi());
//      Serial.print("theta = ");
//      Serial.println(theta);
//
//      while (phi - a < .1) {
//        rotateLeft(100);
//        phi = imuSensor.getPhi();
//      }
//      stopp();
//      break;
//
//    case 4:
//
//      Serial.println("case = 4");
//      Serial.print("phi");
//      Serial.println(imuSensor.getPhi());
//      Serial.print("theta = ");
//      Serial.println(theta);
//      while (abs(phi - a) > 5) {
//        rotateRight(100);
//        phi = imuSensor.getPhi();
//      }
//      stopp();
//      break;
//
//    default:
//      stopp();
//  }
//  return;
//}
//
//
//
//
//void robot::intelFwd(int pwm, int a) {
//
//
//  float phi = imuSensor.getPhi();
//  int pwm_up = pwm + 10;
//  int pwm_dwn = pwm - 10;
//
//  Serial.println("test 0 enter forwards function");
//  if (abs(phi - a) > 1) {
//    align(a);
//  }
//
//  Serial.println("test 6 reentered forwards function");
//  Serial.print("a = ");
//  Serial.println(a);
//  Serial.print("phi = ");
//  Serial.println(phi);
//
//  _M1.forward(pwm);
//  _M2.forward(pwm);
//  _M3.forward(pwm);
//  _M4.forward(pwm);
//
//  if (a != 0) {
//
//    while ((phi > a + .2) && (phi < a + 20))
//    {
//      phi = imuSensor.getPhi();
//      _M2.forward(pwm_up);
//      _M3.forward(pwm_up);
//      _M1.forward(pwm_dwn);
//      _M4.forward(pwm_dwn);
//
//      Serial.println("test 5");
//      Serial.print("pwm_up");
//      Serial.println(pwm_up);
//      Serial.print("pwm_dwn");
//      Serial.println(pwm_dwn);
//      Serial.print("phi =");
//      Serial.println(phi);
//
//    }
//    while ((phi < a - .2) && (phi > a - 20))
//    {
//      phi = imuSensor.getPhi();
//      _M2.forward(pwm_dwn);
//      _M3.forward(pwm_dwn);
//      _M1.forward(pwm_up);
//      _M4.forward(pwm_up);
//
//      Serial.println("test 5");
//      Serial.print("pwm_up");
//      Serial.println(pwm_up);
//      Serial.print("pwm_dwn");
//      Serial.println(pwm_dwn);
//      Serial.print("phi =");
//      Serial.println(phi);
//    }
//  } else {
//    while ((phi > a + .2) && (phi < a + 20))
//    {
//      phi = imuSensor.getPhi();
//      _M2.forward(pwm_up);
//      _M3.forward(pwm_up);
//      _M1.forward(pwm_dwn);
//      _M4.forward(pwm_dwn);
//
//      Serial.println("test 5 a = 0; curving left (1 < phi < 20)");
//      Serial.print("Left motors (M2/M3) pwm =");
//      Serial.println(pwm_up);
//      Serial.print("Right motors (M1/M4) pwm =");
//      Serial.println(pwm_dwn);
//      Serial.print("phi =");
//      Serial.println(phi);
//    }
//    while ((phi < 359.8) && (phi > 340))
//    {
//      phi = imuSensor.getPhi();
//      _M2.forward(pwm_dwn);
//      _M3.forward(pwm_dwn);
//      _M1.forward(pwm_up);
//      _M4.forward(pwm_up);
//
//      Serial.println("test 6 a = 0; curving right (340 < phi < 359)");
//      Serial.print("Left motors (M2/M3) pwm =");
//      Serial.println(pwm_dwn);
//      Serial.print("Right motors (M1/M4) pwm =");
//      Serial.println(pwm_up);
//      Serial.print("phi =");
//      Serial.println(phi);
//    }
//  }
//}
//
//void robot::forwards(int pwm) {
//  _M1.forward(pwm);
//  _M2.forward(pwm);
//  _M3.forward(pwm);
//  _M4.forward(pwm);
//}
//
//void robot::backwards(int pwm) {
//  _M1.backward(pwm);
//  _M2.backward(pwm);
//  _M3.backward(pwm);
//  _M4.backward(pwm);
//}
//void robot::right(int pwm) {
//  _M1.backward(pwm);
//  _M2.forward(pwm);
//  _M3.backward(pwm);
//  _M4.forward(pwm);
//}
//void robot::left(int pwm) {
//  _M1.forward(pwm);
//  _M2.backward(pwm);
//  _M3.forward(pwm);
//  _M4.backward(pwm);
//}
//void robot::rotateRight(int pwm) {
//  _M1.backward(pwm);
//  _M2.forward(pwm);
//  _M3.forward(pwm);
//  _M4.backward(pwm);
//}
//void robot::rotateLeft(int pwm) {
//  _M1.forward(pwm);
//  _M2.backward(pwm);
//  _M3.backward(pwm);
//  _M4.forward(pwm);
//}
//void robot::diagonalLeftUp(int pwm) {
//  _M1.forward(pwm);
//  _M3.forward(pwm);
//}
//void robot::diagonalRightUp(int pwm) {
//  _M2.forward(pwm);
//  _M4.forward(pwm);
//}
//robot::moveInCircular1 (int pwm1, int pwm2) {
//
//  if (pwm1 > 0 && pwm2 > 0) {
//    _M1.forward(pwm1);
//    _M2.forward(pwm2);
//    _M3.forward(pwm1);
//    _M4.forward(pwm2);
//
//  } else if (pwm1 <= 0 && pwm2 >= 0) {
//
//    pwm1 = -pwm1;
//    _M1.backward(pwm1);
//    _M2.forward(pwm2);
//    _M3.backward(pwm1);
//    _M4.forward(pwm2);
//
//  } else if (pwm1 >= 0 && pwm2 <= 0) {
//    pwm2 = -pwm2;
//    _M1.forward(pwm1);
//    _M2.backward(pwm2);
//    _M3.forward(pwm1);
//    _M4.backward(pwm2);
//
//  } else if (pwm1 < 0 && pwm2 < 0) {
//    pwm1 = -pwm1;
//    pwm2 = -pwm2;
//    _M1.backward(pwm1);
//    _M2.backward(pwm2);
//    _M3.backward(pwm1);
//    _M4.backward(pwm2);
//
//  } else {
//    stopp();
//  }
//
//}
//void robot::moveInCircular(int delay1) {
//
//  int pwm11 = maxSpeed;
//  int pwm22 = maxSpeed;
//
//  for ( ; pwm11 > 50 ; pwm11 -= 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//
//  pwm11 = -pwm11;
//  for ( ; pwm11 > -maxSpeed ; pwm11 -= 2) {
//
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//
//  for (; pwm22 > 50 ; pwm22 -= 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//  pwm22 = -pwm22;
//  for (; pwm22 > -maxSpeed ; pwm22 -= 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//
//  for ( ; pwm11 < -50 ; pwm11 += 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//  pwm11 = -pwm11;
//  for ( ; pwm11 < maxSpeed ; pwm11 += 2) {
//
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//  for (; pwm22 < -50 ; pwm22 += 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//  pwm22 = -pwm22;
//  for (; pwm22 < maxSpeed ; pwm22 += 2) {
//    moveInCircular1 (pwm11, pwm22);
//    Serial.print(pwm11 );
//    Serial.print(" : " );
//    Serial.println(pwm22 );
//    delay(delay1);
//  }
//  Serial.print(pwm11 );
//  Serial.print(" : " );
//  Serial.println(pwm22 );
//}
//void robot::stopp() {
//  _M1.stopp();
//  _M2.stopp();
//  _M3.stopp();
//  _M4.stopp();
//}
//void robot::rightWheels(int pwm1)
//{
//  _M1.forward(pwm1);
//  _M4.forward(pwm1);
//}
//void robot::leftWheels(int pwm1)
//{
//  _M2.forward(pwm1);
//  _M3.forward(pwm1);
//}
