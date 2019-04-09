#include "Arduino.h"
#include <Wire.h>
#include "src/laser_ds.h"
#include "src/imu.h"
#include "src/motor.h"
//#include "robot.h"

// Encoder definitions
#define ENCODER_1 6
#define ENCODER_2 7
#define ENCODER_3 3
#define ENCODER_4 2
#define ENCODEROUTPUT 1200

int number = 0;
//int state = 0;


volatile long encoderValue1 = 0;
volatile long encoderValue2 = 0;
volatile long encoderValue3 = 0;
volatile long encoderValue4 = 0;

Lasers lasers(11, 13, 12, 10); // (F, B, L, R)
Imu imu;
/*
  Red  motor power (connects to one motor terminal)
  Black motor power (connects to the other motor terminal)
  Green encoder GND
  Blue  encoder Vcc (3.5 – 20 V)
  Yellow  encoder A output
  White encoder B output
*/

Motor motor1(44, 22, 24);//front right
Motor motor2(5, 51, 52); //front left
Motor motor3(46, 50, 48);//back left
Motor motor4(45, 26, 28);//back right

void count1();
void count2();
void count3();
void count4();

//void forwards(int pwm);
//void backwards(int pwm);
//void right(int pwm);
//void left(int pwm);
//void rotateRight(int pwm);
//void rotateLeft(int pwm);
//void diagonalLeftUp(int pwm);
//void diagonalRightUp(int pwm);
char cc;
//void stp();
//void printData();
//void receiveCommand();
//void sendCommand(char a);
//void firstStep();
//void sendData();
//void receiveData();
//void parseData();
//void showParsedData();
boolean rotating = false;
boolean stepOne = false;
float xCor = 0.0;
float yCor = 0.0;
float objSize = 0.0;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
//void my_go(int base_run, int change); 
/*
  Arduino 5V to shield 4
  Arduino GND to shield 6
  Arduino 18 (TX1) to shield 10
  Arduino 19 (RX1) to shield 8
*/

#define MVAL 25.

void count1(){encoderValue1++;}
void count2(){encoderValue2++;}
void count3(){encoderValue3++;}
void count4(){encoderValue4++;}
void control();
void setup()
{
  //initialization of i2c for sensors
  Wire.begin();
  Serial.begin(115200);
  Serial.flush();
  Serial1.begin(115200);//mega-odroid
  Serial1.flush();
  Serial3.begin(115200);//mega-nano
  Serial3.flush();

  //  Init Lasers dist sensors
  lasers.setAddy();
  delay(100);
  lasers.i2cScan();
  imu.start();

  // Init Encoders
  pinMode(ENCODER_1, INPUT);
  pinMode(ENCODER_2, INPUT);
  pinMode(ENCODER_3, INPUT);
  pinMode(ENCODER_4, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1), count1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2), count2, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_3), count3, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_4), count4, RISING);
  rotating = false;

}

void loop()
{
  //printData();
 control();
//  forwards(100);
//  for(int i=0;i<150;i+=5){
////    motor1.setVal(-i);
////    motor4.setVal(-i);
////    motor2.setVal(i);
////    motor3.setVal(i);
//    Serial.println(i);
//    delay(500);
//  }
//  serialComm2();
}

void my_go(int base_run, int change) {
  // Baserun from 0-100, change -100 - 100
  base_run = clamp<int>(base_run, 0, 100);
  change = clamp<int>(change, -100, 100);

  // lval, rval
  int v[] = {base_run - change, base_run + change};
  
  Serial.print("Base run ");
  Serial.print(base_run);
  Serial.print(" Change:");
  Serial.println(change);

  for(int i=0;i<2;i++){
    if(v[i]<0)v[i]-=1170/MVAL;else v[i]+=1170/MVAL;
    v[i] = clamp<int>(v[i],-1500/MVAL,1500/MVAL);
  }
  
//  if(base_run < 5){
//    for(int i=0;i<2;i++)if(v[i]<0)v[i]-=75;else v[i]+=75;
//  }else{
//    
//  }
  
  motor1.setVal(v[0]);
  motor2.setVal(v[1]);
  motor3.setVal(v[1]);
  motor4.setVal(v[0]);
}


class CommObject {
  public:
    float x = 0.0, y = 0.0, s = 0.0;
    bool square = false;
    uint8_t color = 0;
    // uint8_t type=0; // If 0 bit is on, then square else sphere, 1-7th bits are used for colors, since only 4 colors only 6-7 bits will be used\
    meaning blue, green, red, yellow, in that order.
    void print(){
        Serial.println("");
        Serial.print(x);
        Serial.print(" ");
        Serial.print(y);
        Serial.print(" ");
        Serial.print(s);
        Serial.print(" ");
        Serial.print(square);
        Serial.print(" ");
        Serial.print(color);
        Serial.println(" ");
      }
    static CommObject* getObjects(uint8_t* d, int n, int* out_n);
    // static uint8_t* getBytes(std::vector<CommObject> objects, int* n);
};

CommObject* CommObject::getObjects(uint8_t* d, int n, int* out_n) {
  int osize = (4 * 3 + 1); *out_n = n / osize;
  CommObject *o = new CommObject[*out_n];
  for (int i = 0; i < *out_n; i++) {
    uint8_t* j = d + osize * i;
    o[i].x = *((float*)(j));
    o[i].y = *((float*)(j + 4));
    o[i].s = *((float*)(j + 8));
    o[i].square = (*(j + 12) & (1 << 7)) ? 1 : 0;
    o[i].color = *(j + 12) & 3;
  }
  return o;
}



unsigned long c_time = 0;
int disF, disB, disL, disR;

CommObject* objects;
int obj_n=0;

float imu_angle=0;

void rotate(float angle){
  float diff;
  do{
    imu_update();
    diff = -wrap<float>(imu_angle - angle, -180, 180);
    Serial.println(String(imu_angle)+ " "+ String(diff));
    my_go(0, clamp<float>(diff/10,-10,10));
  }while(abs(diff) > 10);
  stp();
}
CommObject otarget;
void go_obj(){
  while(!get_comm()){}
//  while(1){
  do{
    if(get_comm()){
//      Serial.println(obj_n);
      for(int i=0;i<obj_n;i++){
//        objects[i].print();
        if (!objects[i].square && objects[i].color == 0){
          c_time = millis();
          memcpy(&otarget, &objects[i], sizeof(CommObject));
          otarget.print();
          my_go(objects[i].s < 0.21 ? 1. / objects[i].s : 0, (int)((objects[i].x - 0.5) * 40));
          break;
        }
      }
    }else
      if (millis() - c_time > 50)stp();
//  }
  }while(
//    true
    (otarget.s < 0.2 || otarget.s > 0.24) || (millis() - c_time > 50)
    );
  stp();
}
// Non-blocking
void go_round(){
  imu_update();
  
  
}

enum State{Static=0, Finding, Picking, Placing};
State state_f = Finding;


//***** Sensors update functions
void imu_update(){
  // Reading angle
  imu_angle = imu.getPhi();
}
void laser_update(){
  // Reading lasers
  disF = lasers.sensF();disB = lasers.sensB();disL = lasers.sensL();disR = lasers.sensR();
}
void camera_update(){
  // Getting objects from camera
  if(get_comm()){
    // We got some objects...
//    c_time = millis();
  }
}
//*******

void control(){
//  Serial.println("1");
  switch(state_f){
    case Static:
      Serial.println("2");
      imu_update();
      rotate(wrap<float>(imu_angle - 45, 0, 360));
      state_f = Finding;
      Serial.println("Passed to finding");
      break;
    case Finding:
      
      go_obj();
      
      Serial.println("Passed to picking");
      state_f = Picking;
      break;
    case Picking:
      
      break;
    case Placing:
      
      break;
    default:
      Serial.println(state_f);
  }
//  if (millis() - c_time > 50)stp();
  
}

unsigned char char_buff[300];
int c_counter = 0;
bool get_comm()
{
  
  bool camera_done = false;
  while (Serial3.available())
  {
    Serial3.readBytes(char_buff + c_counter, 1);
    char c = char_buff[c_counter]; // So we just added a new character to our buffer

    if (c == 'e') {
      if (char_buff[c_counter - 1] == '\n') {
        // Checksum and done
        camera_done = true;
        c_counter++;
        break;
      }
    }
    c_counter++;
  }
  bool checksum_good = false;
  if (camera_done) {
    auto checksum_val = *((uint16_t*)(char_buff + c_counter - 4));
    //    Serial.println(c_counter-4 == *((uint16_t*)(char_buff+c_counter-4)) ? "Checksum passed" : "Checksum failed " + String(c_counter) + " " + String(*((uint16_t*)(char_buff+c_counter-4))));
    if (c_counter - 4 == checksum_val) {
      checksum_good = true;
      Serial.println("Good check");
      if (objects)delete[] objects;
      objects = CommObject::getObjects(char_buff, c_counter - 4, &obj_n);
//      Serial.println(obj_n);
//      for(int i=0;i<obj_n;i++){
//        objects[i].print();
//      }
    }else Serial.println("Bad checksum " + String(c_counter - 4) + " " + String(checksum_val) );
    c_counter = 0;
  }
//  Serial.println(camera_done && checksum_good);
  return camera_done && checksum_good;
}

template<typename T>
T clamp(T v, T l, T h){if(v<l)return l;else if(v>h) return h;else return v;}

template<typename T>
T wrap(T v, T l, T h){
  if(l>h)return v;else {
    T r=h-l;
    while (v<l)v+=r;
    while (v>h)v-=r;
  }
  return v;
}


//void receiveCommand(char oComm)
//{
//  switch (oComm)
//  {
//    case 's':
//      firstStep();
//      Serial.println("Case 0");
//      break;
//    case '1':
//      //stp();
//      forwards(65);
//      Serial.println("Case 1");
//      break;
//    case '2':
//      //stp();
//      rotateRight(65);
//      Serial.println("Case 2");
//      break;
//    case '3':
//      Serial.println("Case 3");
//      break;
//    case '4':
//      //stp();
//      rotateLeft(65);
//      Serial.println("Case 4");
//      break;
//    case 'z':
//      stp();
//      Serial.println("Case 5");
//      break;
//    default:
//      stp();
//      Serial.println("Case default");
//      break;
//  }
//}
//void firstStep()
//{
//  float angle = imu.getPhi();
//  Serial.println(angle);
//  if (!stepOne)
//  {
//    while (angle <= 60)
//    {
//      angle = imu.getPhi();
//      Serial.print("Angle inside:");
//      Serial.println(angle);
//      if (!rotating)
//      {
//        rotateRight(70);
//        rotating = true;
//      }
//      delay(10);
//    }
//    stp();
//    rotating = false;
//    stepOne = true;
//  }
//}
void sendCommand(char a)
{
  if (Serial1.available())
  {
    Serial1.write(a);
  }
}

void printData()
{
  int disF = lasers.sensF();
  int disB = lasers.sensB();
  int disL = lasers.sensL();
  int disR = lasers.sensR();

  Serial.print("disF = ");
  Serial.println(disF);
  Serial.print("disB = ");
  Serial.println(disB);
  Serial.print("disL = ");
  Serial.println(disL);
  Serial.print("disR = ");
  Serial.println(disR);
  float phi = imu.getPhi();
  Serial.print("phi = ");
  Serial.println(phi);
  Serial.println();
  delay(100);
}

int interval = 1000;
long previousMillis = 0;
long currentMillis = 0;
int rpm1 = 0;
int rpm2 = 0;
int rpm3 = 0;
int rpm4 = 0;
void rpm()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = millis();
    rpm1 = (encoderValue1 * 60 / ENCODEROUTPUT);
    rpm2 = (encoderValue2 * 60 / ENCODEROUTPUT);
    rpm3 = (encoderValue3 * 60 / ENCODEROUTPUT);
    rpm4 = (encoderValue4 * 60 / ENCODEROUTPUT);
    Serial.print("RPM_1 = "); Serial.println(rpm1);
    Serial.print("RPM_2 = "); Serial.println(rpm2);
    Serial.print("RPM_3 = "); Serial.println(rpm3);
    Serial.print("RPM_4 = "); Serial.println(rpm4);
    Serial.print("Encoder Value1 = "); Serial.println(encoderValue1);
    Serial.print("Encoder Value2 = "); Serial.println(encoderValue2);
    Serial.print("Encoder Value3 = "); Serial.println(encoderValue3);
    Serial.print("Encoder Value4 = "); Serial.println(encoderValue4);
    encoderValue1 = 0;
    encoderValue2 = 0;
    encoderValue3 = 0;
    encoderValue4 = 0;
  }

}


/*
   function that executes whenever data is received from master
   this function is registered as an event, see setup()
*/

// Right 1 and 4
// Left 2 and 3
//void forwards(int pwm)
//{
//  motor1.forward(pwm);
//  motor2.forward(pwm);
//  motor3.forward(pwm);
//  motor4.forward(pwm);
//}
//
//void backwards(int pwm)
//{
//  motor1.backward(pwm);
//  motor2.backward(pwm);
//  motor3.backward(pwm);
//  motor4.backward(pwm);
//}
//
//void rotateRight(int pwm)
//{
//  motor1.backward(pwm);
//  motor2.forward(pwm);
//  motor3.forward(pwm);
//  motor4.backward(pwm);
//}
//
//void rotateLeft(int pwm)
//{
//  motor1.forward(pwm);
//  motor2.backward(pwm);
//  motor3.backward(pwm);
//  motor4.forward(pwm);
//}
//
//void diagonalLeftUp(int pwm)
//{
//  motor1.forward(pwm);
//  motor3.forward(pwm);
//}
//
//void diagonalRightUp(int pwm)
//{
//  motor2.forward(pwm);
//  motor4.forward(pwm);
//}

void stp() {
  motor1.stp();
  motor2.stp();
  motor3.stp();
  motor4.stp();
}
