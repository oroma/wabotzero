#include <SoftwareSerial.h>

SoftwareSerial inSerial(2,3);

#define MAX_CMD_COUNT 7

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  inSerial.begin(9600);
}

int doMovingByPreset(unsigned char preset)
{
  Serial.print("preset: ");
  Serial.print(preset, DEC);

  return preset;
}

int doMovingJointArm(char lr, unsigned char j0, unsigned char j1, unsigned char j2)
{
  Serial.print("manual: ");
  if (lr == 'L') {
    Serial.print("left, ");
  } else if (lr == 'R') {
    Serial.print("right, ");
  }
  Serial.print(j0, DEC);
  Serial.print(' ');
  Serial.print(j1, DEC);
  Serial.print(' ');
  Serial.print(j2, DEC);

  return lr;
}

int doMovingJointDisc(unsigned char value)
{
  Serial.print("manual: disc, ");
  Serial.print(value, DEC);

  return value;
}

int doMovingWheel(unsigned char d, unsigned char m)
{
  return d;
}

char testCmds[MAX_CMD_COUNT][10] = {
  { 'P', 0x01, '\n' },
  { 'P', 0x02, '\n' },
  { 'M', 'J', 'A', 'L', 0xB4, 0xB4, 0xB4,'\n' },
  { 'M', 'J', 'A', 'R', 0xB4, 0xB4, 0xB4,'\n' },
  { 'M', 'J', 'D', 0xB4, '\n' },
  { 'M', 'W', 'D', 'F', '\n' },
  { 'M', 'W', 'D', 'B', '\n' }
};

void loop() {
  for (int i; i < MAX_CMD_COUNT; i++) {
    if (testCmds[i][0] == 'M') {
      if (testCmds[i][1] == 'J' && testCmds[i][2] == 'A')
        doMovingJointArm(testCmds[i][3], testCmds[i][4], testCmds[i][5], testCmds[i][6]);
      if (testCmds[i][1] == 'J' && testCmds[i][2] == 'D')
        doMovingJointDisc(testCmds[i][3]);
      if (testCmds[i][1] == 'W' && testCmds[i][2] == 'D')
        doMovingWheel(testCmds[i][3], testCmds[i][4]);
    } else if (testCmds[i][0] == 'P') {
      doMovingByPreset(testCmds[i][1]);
    }

    Serial.println();
  }
  while (Serial1.available()) {
   
      Serial.print(Serial1.read(), HEX);
      Serial.println();
  }
}
