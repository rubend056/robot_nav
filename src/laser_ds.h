#ifndef LDS
#define LDS

class Lasers {

  public:
    //  Constructor
    Lasers(int F_pin, int B_pin, int L_pin, int R_pin);

    //  Methods
    void setAddy();
    void i2cScan();
    int sensF();
    int sensB();
    int sensL();
    int sensR();
  private:

    int _pinF;
    int _pinB;
    int _pinL;
    int _pinR;

};

#endif
