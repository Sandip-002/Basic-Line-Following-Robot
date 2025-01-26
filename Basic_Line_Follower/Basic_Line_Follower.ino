#define led 13

byte channel_adress_table[14][4] = {
  { 1, 0, 1, 1 },  //0 - 11
  { 0, 0, 1, 1 },  //1 - 3
  { 1, 1, 0, 1 },  //2 - 13
  { 0, 1, 0, 1 },  //3 - 5
  { 1, 0, 0, 1 },  //4 - 9
  { 0, 0, 0, 1 },  //5 - 1
  { 1, 1, 1, 0 },  //6 - 14
  { 0, 1, 1, 0 },  //7 - 6
  { 1, 0, 1, 0 },  //8 - 10
  { 0, 0, 1, 0 },  //9 - 2
  { 1, 1, 0, 0 },  //10 - 12
  { 0, 1, 0, 0 },  //11 - 4
  { 1, 0, 0, 0 },  //12 - 8
  { 0, 0, 0, 0 },  //13 - 0
};
//mux variables
#define S0 A1
#define S1 A2
#define S2 A3
#define S3 A4

//========button pins========
#define cal 4
#define runbot 5

//=====motor pins=======
//MOTOR 1
#define lmf 8
#define lmb 7
#define lmPWM 6
//MOTOR 2
#define rmf 9
#define rmb 10
#define rmPWM 11

//PID variables
float Kp, Ki, Kd;
int error;
int lastError = 0;

int baseSpeedC = 200, maxSpeed = 250,  baseSpeed;
int brkSpeed, minSpeed;
int lmSpeed, rmSpeed;
byte flag = 0, mode=0;
//=======IR variables=======
#define numSensor 14
int rawSensor[numSensor], maximum[numSensor], minimum[numSensor], mid[numSensor], sensor[numSensor];
uint16_t position = 0;
byte on_line = 0;
int maxTh = 500, minTh = 400;
#include <EEPROM.h>

void setup() {
  // put your setup code here, to runbot once:
  Serial.begin(9600);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(A0, INPUT);

  pinMode(cal, INPUT_PULLUP);
  pinMode(runbot, INPUT_PULLUP);

  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);

  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);

  pinMode(lmPWM, OUTPUT);
  pinMode(rmPWM, OUTPUT);

  analogReadSetup();

}

void loop() {
//  // put your main code here, to runbot repeatedly:
  if(push(cal))
  {
    if(!push(runbot)) mode = 1;   
  }
  else if(push(runbot))
  {
    if(!push(cal)) mode = 2;   
  }
  else
    mode = 0;
  switch (mode)
  {
    case 1:
      Serial.println("Calibration");
      calibration();
      break;
    case 2:
      Serial.println("Line follow");
      while (1)
      {
        //long st = micros();
        position = readLine();
        printReading(0);
        if (!on_line)
        {
          if (!on_line) //if all sensor on white(dotted line) it'll go forward 80ms (tune this value)
          {
            drive(lmSpeed, rmSpeed);
            delay(80);
          }
        }
        else if(on_line == numSensor) break;
        
        lineFollow(
          position,
          baseSpeedC = 100,
          maxSpeed = 180,
          minSpeed = -30,
          Kp = 0.02,
          Kd = 0.9);
      }
      break;
    default:
      Serial.println("default");
      drive(0, 0);
  }
}

byte push(byte button_number)
{
  uint32_t b_timer = 0;
  if (!digitalRead(button_number))
  { //if button is pressed
    delay(50);                        //debounce delay;
    while (!digitalRead(button_number))
    {
      delay(50);
      b_timer += 50;
      if (b_timer > 1000) digitalWrite(led, 1);
    }
    digitalWrite(led,0);
    if (b_timer > 1000) return 2;    //long press
    else if (b_timer > 0) return 1;  //short press
  }
  return 0;
}
