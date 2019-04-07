#include "Arduino.h"
#include <Wire.h>
#include "laser_ds.h"
#include "imu.h"
#include "robot.h"

// Encoder definitions
#define ENCODER_1 6
#define ENCODER_2 7
#define ENCODER_3 3
#define ENCODER_4 2
#define ENCODEROUTPUT 1200

int number = 0;
int state = 0;
int interval = 1000;
long previousMillis = 0;
long currentMillis = 0;
int rpm1 = 0;
int rpm2 = 0;
int rpm3 = 0;
int rpm4 = 0;

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

void forwards(int pwm);
void backwards(int pwm);
void right(int pwm);
void left(int pwm);
void rotateRight(int pwm);
void rotateLeft(int pwm);
void diagonalLeftUp(int pwm);
void diagonalRightUp(int pwm);
char cc;
void stp();
void printData();
void receiveCommand();
void sendCommand(char a);
void firstStep();
void sendData();
void receiveData();
void parseData();
void showParsedData();
boolean rotating = false;
boolean stepOne = false;
float xCor = 0.0;
float yCor = 0.0;
float objSize = 0.0;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
void my_go(int base_run, int change); 
/*
  Arduino 5V to shield 4
  Arduino GND to shield 6
  Arduino 18 (TX1) to shield 10
  Arduino 19 (RX1) to shield 8
*/

void setup()
{
  //initialization of i2c for sensors
  Wire.begin();
  Serial.begin(115200);
  //Serial communication odriod-arduino
  Serial.flush();
  Serial1.begin(115200);//mega-odroid
  Serial1.flush();
  Serial2.begin(115200);//mega-nano
  Serial2.flush();

  //  Init Lasers dist sensors
  lasers.setAddy();
  delay(100);
  lasers.i2cScan();
  //  Init IMU
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
  //delay(5000);
  //firstStep();

}

void loop()
{
  //printData();
  //forwards(100);
  for(int i=0;i<150;i+=5){
//    motor1.setVal(-i);
//    motor4.setVal(-i);
//    motor2.setVal(i);
//    motor3.setVal(i);
    Serial.println(i);
    delay(500);
  }
//  serialComm2();
}

void serialComm()
{
  //  if(Serial1.available())
  //  {
  //    String newLine="";
  //    char cNew;
  //    cNew = Serial.read();
  //    newLine.concat(cNew);
  //    cNew=Serial.read();
  //    newLine.concat(cNew);
  //    if(newLine.compareTo("\n") == 0)
  //    {
  //      Serial.println("GotLINE?");
  //      byte xData[4];
  //      xData[0] = Serial.read();
  //      xData[1] = Serial.read();
  //      xData[2] = Serial.read();
  //      xData[3] = Serial.read();
  //      xCor = *(float*)(xData);
  //      Serial.print("x=");Serial.print(xCor);
  //
  //
  //      xData[0] = Serial.read();
  //      xData[1] = Serial.read();
  //      xData[2] = Serial.read();
  //      xData[3] = Serial.read();
  //      yCor = *(float*)(xData);
  //      Serial.print(", y=");Serial.print(yCor);
  //
  //      xData[0] = Serial.read();
  //      xData[1] = Serial.read();
  //      xData[2] = Serial.read();
  //      xData[3] = Serial.read();
  //      objSize = *(float*)(xData);
  //      Serial.print(", size=");Serial.print(objSize);
  //
  //      uint16_t chkSum;
  //      byte cData[2];
  //      cData[0] = Serial.read();
  //      cData[1] = Serial.read();
  //
  //      chkSum = *(uint16_t *)(cData);
  //      Serial.print("\ncheckSum=");Serial.println((int)chkSum);
  //    }
  //  }

  if (Serial1.available())
  {
    cc = tolower(Serial1.read());
    //receiveCommand(cc);
  }

  if (Serial2.available())
  {
    cc = tolower(Serial2.read());
    Serial.println(cc);
    cc = 'a';
    Serial2.write(cc);
  }
}


void my_go(int base_run, int change) {

  if (abs(change) > 40 || base_run > 40) {
    Serial.print("Base run ");
    Serial.print(base_run);
    Serial.print(" Change:");
    Serial.println(change);

    int lval = base_run + change;
    motor1.setVal(base_run - change);
//    motor2.setVal();
    motor3.setVal(base_run + change);
    motor4.setVal(base_run - change);
  } else stp();
}

class CommObject {
  public:
    float x = 0, y = 0, s = 0;
    bool square = false;
    uint8_t color = 0;
    // uint8_t type=0; // If 0 bit is on, then square else sphere, 1-7th bits are used for colors, since only 4 colors only 6-7 bits will be used\
    meaning blue, green, red, yellow, in that order.
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

unsigned char char_buff[300];
int c_counter = 0;
bool done_read = false;
unsigned long done_time = 0;

void serialComm2()
{
  done_read = false;
  if (Serial2.available())
  {
    Serial2.readBytes(char_buff + c_counter, 1);
    char c = char_buff[c_counter]; // So we just added a new character to our buffer

    if (c == 'e') {
      if (char_buff[c_counter - 1] == '\n') {
        // Checksum and done
        done_read = true;
      }
    }

    c_counter++;
  }
  if (done_read) {
    
    //    Serial.println(c_counter-4 == *((uint16_t*)(char_buff+c_counter-4)) ? "Checksum passed" : "Checksum failed " + String(c_counter) + " " + String(*((uint16_t*)(char_buff+c_counter-4))));
    if (c_counter - 4 == *((uint16_t*)(char_buff + c_counter - 4))) {
      int n;
      CommObject* objects = CommObject::getObjects(char_buff, c_counter - 4, &n);
      Serial.println("");
      Serial.print(objects[0].x);
      Serial.print(" ");
      Serial.print(objects[0].y);
      Serial.print(" ");
      Serial.print(objects[0].s);
      Serial.print(" ");
      Serial.print(objects[0].square);
      Serial.print(" ");
      Serial.print(objects[0].color);
      Serial.println(" ");

      for(int i=0;i<n;i++){
        if (!objects[i].square && objects[i].color == 0){
          done_time = millis();
          my_go(objects[i].s < 0.15 ? clamp(7 / objects[i].s,0,120) : 0, (int)((objects[i].x - 0.5) * 200));
          break;
        }
      }
      
      delete[] objects;
    }
    c_counter = 0;
  }

  if (millis() - done_time > 100)stp();
}

int clamp(int v, int l, int h){if(v<l)return l;else if(v>h) return h;}


void receiveCommand(char oComm)
{
  switch (oComm)
  {
    case 's':
      firstStep();
      Serial.println("Case 0");
      break;
    case '1':
      //stp();
      forwards(65);
      Serial.println("Case 1");
      break;
    case '2':
      //stp();
      rotateRight(65);
      Serial.println("Case 2");
      break;
    case '3':
      Serial.println("Case 3");
      break;
    case '4':
      //stp();
      rotateLeft(65);
      Serial.println("Case 4");
      break;
    case 'z':
      stp();
      Serial.println("Case 5");
      break;
    default:
      stp();
      Serial.println("Case default");
      break;
  }
}
void firstStep()
{
  float angle = imu.getPhi();
  Serial.println(angle);
  if (!stepOne)
  {
    while (angle <= 60)
    {
      angle = imu.getPhi();
      Serial.print("Angle inside:");
      Serial.println(angle);
      if (!rotating)
      {
        rotateRight(70);
        rotating = true;
      }
      delay(10);
    }
    stp();
    rotating = false;
    stepOne = true;
  }



}
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
  delay(1000);
}


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
void count1()
{
  encoderValue1++;
}
void count2()
{
  encoderValue2++;
}
void count3()
{
  encoderValue3++;
}
void count4()
{
  encoderValue4++;
}

/*
   function that executes whenever data is received from master
   this function is registered as an event, see setup()
*/

// Right 1 and 4
// Left 2 and 3
void forwards(int pwm)
{
  motor1.forward(pwm);
  motor2.forward(pwm);
  motor3.forward(pwm);
  motor4.forward(pwm);
}

void backwards(int pwm)
{
  motor1.backward(pwm);
  motor2.backward(pwm);
  motor3.backward(pwm);
  motor4.backward(pwm);
}

void rotateRight(int pwm)
{
  motor1.backward(pwm);
  motor2.forward(pwm);
  motor3.forward(pwm);
  motor4.backward(pwm);
}

void rotateLeft(int pwm)
{
  motor1.forward(pwm);
  motor2.backward(pwm);
  motor3.backward(pwm);
  motor4.forward(pwm);
}

void diagonalLeftUp(int pwm)
{
  motor1.forward(pwm);
  motor3.forward(pwm);
}

void diagonalRightUp(int pwm)
{
  motor2.forward(pwm);
  motor4.forward(pwm);
}

void stp() {
  motor1.stp();
  motor2.stp();
  motor3.stp();
  motor4.stp();
}
