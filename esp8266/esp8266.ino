/*
 * NodeMCU/ESP8266 act as AP (Access Point) and simplest Web Server
 * to control GPIO (on-board LED)
 * Connect to AP "arduino-er", password = "password"
 * Open browser, visit 192.168.4.1
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

const char *ssid = "arduino-er";
const char *password = "password";

ESP8266WebServer server(80);

const byte rxPin = 14;
const byte txPin = 12;
SoftwareSerial toMega(rxPin, txPin);

void handleRoot()
{
  Serial.println(server.uri());
  Serial.println(server.method());
  Serial.println(server.args());

  response();
}

void handleCommand()
{
  Serial.println(server.uri());
  Serial.println(server.method());
  Serial.println(server.args());

  response();
}

void handleJointCommand()
{
  Serial.println(server.uri());
  Serial.println(server.method());
  Serial.println(server.args());

  // range: 30 ~ 150
  int errorCode = 200;
  char cmd[8] = {'M', 'J', 'A', 'L', 0xff, 0xff, 0xff, '\0'};

  // e.g.) http://192.168.4.1/command/joint?part=a&d=l&no=2&angle=110
  if (server.hasArg("part") &&
      server.hasArg("d") &&
      server.hasArg("no") &&
      server.hasArg("angle"))
  {
    cmd[2] = toupper(server.arg("part")[0]); // part
    cmd[3] = toupper(server.arg("d")[0]);    // direction
    cmd[4 + server.arg("no").toInt()] = (unsigned char)server.arg("angle").toInt();
  }
  else
  {
    errorCode = 400; // Bad Request
  }
  Serial.println(cmd);

  // send to mega
  int res = toMega.print(cmd);
  Serial.print(res, DEC);

  String r = "" + res;
  String message = "";
  message += "{ \"uri\": ";
  message += "\"" + server.uri() + "\",";
  message += "\"method\": \"";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\",";
  message += "\"command\": \"";
  message += cmd;
  message += "\",";
  message += "\"written\": \"";
  message += "" + res;
  message += "\",";
  message += "\"arguments\": [";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " { \"" + server.argName(i) + "\": \"" + server.arg(i) + "\" }";
    if (i < server.args() - 1)
      message += ", ";
  }
  message += "]}";

  server.send(errorCode, "application/json", message);
}

void handleWheelCommand()
{
  Serial.println(server.uri());
  Serial.println(server.method());
  Serial.println(server.args());

  response();
}

void handlePresetCommand()
{
  Serial.println(server.uri());
  Serial.println(server.method());
  Serial.println(server.args());

  // range: 30 ~ 150
  int errorCode = 200;
  char cmd[3] = {'P', 0xff, '\0'};

  // e.g.) http://192.168.4.1/preset/act=1
  if (server.hasArg("act"))
  {
    cmd[1] = (unsigned char)server.arg("act").toInt();
  }
  else
  {
    errorCode = 400; // Bad Request
  }
  Serial.println(cmd);

  // send to mega
  int res = toMega.print(cmd);
  Serial.print(res, DEC);

  response();
}

void handleNotFound()
{
  String message = "";
  message += "{ \"uri\": ";
  message += "\"" + server.uri() + "\",";
  message += "\"method\": \"";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\",";
  message += "\"arguments\": [";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " { \"" + server.argName(i) + "\": \"" + server.arg(i) + "\" }";
    if (i < server.args() - 1)
      message += ", ";
  }
  message += "]}";

  server.send(404, "application/json", message);
}

void response()
{
  String htmlRes = "{ \"result\": true }";
  server.send(200, "application/json", htmlRes);
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  toMega.begin(9600);

  toMega.print('a');
  Serial.println();

  WiFi.softAP(ssid, password);

  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
  if (MDNS.begin("esp8266.local"))
  {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/command", handleCommand);
  server.on("/command/joint", handleJointCommand);
  server.on("/command/wheel", handleWheelCommand);
  server.on("/preset", handlePresetCommand);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server beginned");
}

void loop()
{
  server.handleClient();
}
