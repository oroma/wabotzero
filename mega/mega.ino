#include <SoftwareSerial.h>
#include <Servo.h>                   //add '<' and '>' before and after servo.h
#include <Wire.h>                    // I2C Control
#include <Adafruit_PWMServoDriver.h> // PCA9685 Servo Drive Board

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // this is the 'maximum' pulse length count (out of 4096)

#define WheelBack1  2
#define WheelBack2  3
#define WheelFront1 4
#define WheelFront2 5

#define WheelLeft1  6
#define WheelLeft2  7
#define WheelRight1 8
#define WheelRight2 9

#define MAX_SPEED 100 // TODO: fix me
#define TurnMAX_SPEED 50

#define ACCEL 1 //속도 증가

int joint_l_0_pin = 0; // PCA9865 Pin 0
int joint_l_1_pin = 1; // PCA9865 Pin 1
int joint_l_2_pin = 2; // PCA9865 Pin 2
int joint_r_0_pin = 13; // PCA9865 Pin 3
int joint_r_1_pin = 14; // PCA9865 Pin 4
int joint_r_2_pin = 15; // PCA9865 Pin 5

Servo joint_l_0;
Servo joint_l_1;
Servo joint_l_2;
Servo joint_r_0;
Servo joint_r_1;
Servo joint_r_2;

#define MAX_PRESET 3
int presetJoint[MAX_PRESET][6] = {
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {0x50, 0x50, 0x50, 0x50, 0x50, 0x50},
    {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}};

int setJointAngle(Servo s, int v);

void setup()
{
  Serial1.begin(115200);
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pwm.setPWM(0, 1024, 0);
  pwm.setPWM(13, 1024, 0);
  pinMode(WheelBack1, OUTPUT);
  pinMode(WheelBack2, OUTPUT);
  pinMode(WheelLeft1, OUTPUT);
  pinMode(WheelLeft2, OUTPUT);
  pinMode(WheelFront1, OUTPUT);
  pinMode(WheelFront2, OUTPUT);
  pinMode(WheelRight1, OUTPUT);
  pinMode(WheelRight2, OUTPUT); // Wheel PWM Control
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}

int doMovingByPreset(unsigned char preset)
{
  

 
}

int setJointAngle(Servo s, int v)
{
  if (v > 0 && v < 0xFF)
  {
    s.write(v);
    Serial.println(v);
  }
  // do sth or not?
}
int doMovingJointArm(char lr, unsigned char j0, unsigned char j1, unsigned char j2)
{
  Serial.print("manual: ");
  if (lr == 'L')
  {
    Serial.println("left");
    setJointAngle(joint_l_0, (int)j0);
    setJointAngle(joint_l_1, (int)j1);
    setJointAngle(joint_l_2, (int)j2);
  }
  else if (lr == 'R')
  {
    Serial.println("right");
    setJointAngle(joint_r_0, (int)j0);
    setJointAngle(joint_r_1, (int)j1);
    setJointAngle(joint_r_2, (int)j2);
  }

  delay(1000);

  return lr;
}

int doMovingJointDisc(unsigned char value)
{
  Serial.print("manual: disc, ");
  Serial.print(value, DEC);

  return value;
}

int setWheelForward(bool on)
{
  for (int i = 0; i < MAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelLeft1, i);
    analogWrite(WheelLeft2, 0);
    analogWrite(WheelRight1, 0);
    analogWrite(WheelRight2, i);
  }
}

int setWheelBackward(bool on)
{
  for (int i = 0; i < MAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelLeft1, 0);
    analogWrite(WheelLeft2, i);
    analogWrite(WheelRight1, i);
    analogWrite(WheelRight2, 0);
  }
}

int setWheelLeft(bool on)
{
  for (int i = 0; i < MAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelFront1, 0);
    analogWrite(WheelFront2, i);
    analogWrite(WheelBack1, i);
    analogWrite(WheelBack2, 0);
  }
}

int setWheelRight(bool on)
{
  for (int i = 0; i < MAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelFront1, i);
    analogWrite(WheelFront2, 0);
    analogWrite(WheelBack1, 0);
    analogWrite(WheelBack2, i);
  }
}

int setWheelTurnLeft(bool on)
{
  for (int i = 0; i < TurnMAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelFront1, 0);
    analogWrite(WheelFront2, i);
    analogWrite(WheelBack1, 0);
    analogWrite(WheelBack2, i);
    analogWrite(WheelLeft1, 0);
    analogWrite(WheelLeft2, i);
    analogWrite(WheelRight1, 0);
    analogWrite(WheelRight2, i);

    
  }
}

int setWheelTurnRight(bool on)
{
  for (int i = 0; i < TurnMAX_SPEED; i += ACCEL)
  {
    analogWrite(WheelFront1, i);
    analogWrite(WheelFront2, 0);
    analogWrite(WheelBack1, i);
    analogWrite(WheelBack2, 0);
    analogWrite(WheelLeft1, i);
    analogWrite(WheelLeft2, 0);
    analogWrite(WheelRight1, i);
    analogWrite(WheelRight2, 0);
 
  }
}

int setWheelStop(bool on)
{
  analogWrite(WheelFront1, 0);
  analogWrite(WheelFront2, 0);
  analogWrite(WheelBack1, 0);
  analogWrite(WheelBack2, 0);
  analogWrite(WheelLeft1, 0);
  analogWrite(WheelLeft2, 0);
  analogWrite(WheelRight1, 0);
  analogWrite(WheelRight2, 0);
}

#define COUNT_FP 5
int doTurnWheel(unsigned char d)
{
  switch (d)
  {
  case 'L':
    setWheelTurnLeft(true);
    break;
  case 'R':
    setWheelTurnRight(true);
    break;
  default:
    break;
  }
}

int doMovingWheel(char d)
{
  int (*wheelMoveFp[COUNT_FP])(bool) = {
      setWheelForward,
      setWheelBackward,
      setWheelLeft,
      setWheelRight,
      setWheelStop};

  int (*wheelTurnFp[2])(bool) = {
      setWheelTurnLeft,
      setWheelTurnRight};

  /*
  int op = (int)m;

  if (d == 'F') {
    
    if (op < 0 || op > COUNT_FP)
      return -1;

    wheelMoveFp[op]((t > 0 ? true : false));

  } else if (d == 'T') {
    if (op < 0 || op > 2)
      return -1;

    wheelTurnFp[op]((t > 0 ? true : false));
  }
  */

  switch (d)
  {
  case 'F':
    setWheelForward(true);
    break;
  case 'B':
    setWheelBackward(true);
    break;
  case 'L':
    setWheelLeft(true);
    break;
  case 'R':
    setWheelRight(true);
    break;
  case 'S':
    setWheelStop(true);
    break;
  default:
    break;
  }
  return d;
}
int dispatchCommand(char cmd[])
{
  if (cmd[0] == 'M')
  {
    if (cmd[1] == 'J' && cmd[2] == 'A')
      doMovingJointArm(cmd[3], cmd[4], cmd[5], cmd[6]);
    if (cmd[1] == 'J' && cmd[2] == 'D')
      doMovingJointDisc(cmd[3]);
    if (cmd[1] == 'W' && cmd[2] == 'D')
      doMovingWheel(cmd[3]);
    if (cmd[1] == 'W' && cmd[2] == 'T')
      doTurnWheel(cmd[3]);
  }
  else if (cmd[0] == 'P')
  {
    doMovingByPreset(cmd[1]);
  }
  return 0;
}

void loop()
{
  String cmd;

  while (Serial1.available())
  {
    Serial.print("connected");
    cmd = Serial1.readString(); // read the incoming data as string
    dispatchCommand(cmd.c_str());
    Serial.println(cmd);
  }
//  pwm.setPWM(0, 0, pulseWidth(0));
//  delay(1000);
//  pwm.setPWM(0, 0, pulseWidth(120));
//  delay(500);
//  pwm.setPWM(0, 0, pulseWidth(90));
//  delay(1000);
}
