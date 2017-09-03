#include <SoftwareSerial.h>
#include <Servo.h> //add '<' and '>' before and after servo.h

int joint_l_0_pin = 4; // pwm pin 4
int joint_l_1_pin = 5; // pwm pin 5
int joint_l_2_pin = 6; // pwm pin 6
int joint_r_0_pin = 7; // pwm pin 7
int joint_r_1_pin = 8; // pwm pin 8
int joint_r_2_pin = 9; // pwm pin 9

Servo joint_l_0;
Servo joint_l_1;
Servo joint_l_2;
Servo joint_r_0;
Servo joint_r_1;
Servo joint_r_2;

#define MAX_PRESET 3
int presetJoint[MAX_PRESET][6] = {
  { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
  { 0x50, 0x50, 0x50, 0x50, 0x50, 0x50 },
  { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }
};

int setJointAngle(Servo s, int v);

void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  joint_l_0.attach(joint_l_0_pin);
  joint_l_1.attach(joint_l_1_pin);
  joint_l_2.attach(joint_l_2_pin);
  joint_r_0.attach(joint_r_0_pin);
  joint_r_1.attach(joint_r_1_pin);
  joint_r_2.attach(joint_r_2_pin);
}

int doMovingByPreset(unsigned char preset)
{
  Serial.print("preset: ");
  Serial.print(preset, DEC);

  if (preset < 0 || preset > MAX_PRESET)
    return -1;
 
  setJointAngle(joint_l_0, presetJoint[preset][0]);
  setJointAngle(joint_l_1, presetJoint[preset][1]);
  setJointAngle(joint_l_2, presetJoint[preset][2]);

  setJointAngle(joint_r_0, presetJoint[preset][3]);
  setJointAngle(joint_r_1, presetJoint[preset][4]);
  setJointAngle(joint_r_2, presetJoint[preset][5]);
    
  return preset;
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
}

int setWheelBackward(bool on)
{
}

int setWheelLeft(bool on)
{
}

int setWheelRight(bool on)
{
}

int setWheelTurnLeft(bool on)
{
}

int setWheelTurnRight(bool on)
{
}

#define COUNT_FP 4
int doMovingWheel(unsigned char d, unsigned char m, unsigned char t)
{
  int (*wheelMoveFp[COUNT_FP])(bool) = {
    setWheelForward,
    setWheelBackward,
    setWheelLeft,
    setWheelRight
  };

  int (*wheelTurnFp[2])(bool) = {
    setWheelTurnLeft,
    setWheelTurnRight
  };

  int op = (int)m;

  if (d == 'D') {
    
    if (op < 0 || op > COUNT_FP)
      return -1;

    wheelMoveFp[op]((t > 0 ? true : false));

  } else if (d == 'T') {
    if (op < 0 || op > 2)
      return -1;

    wheelTurnFp[op]((t > 0 ? true : false));
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
      doMovingWheel(cmd[2], cmd[3], cmd[4]);
    if (cmd[1] == 'W' && cmd[2] == 'T')
      doMovingWheel(cmd[2], cmd[3], cmd[4]);
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
    cmd = Serial1.readString(); // read the incoming data as string
    dispatchCommand(cmd.c_str());
    Serial.println(cmd);
  }
}
