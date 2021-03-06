// an attempt at a generic include file, with minimal globals

#include <arduino.h>
#include <wire.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266FTPServer.h>
#include <LittleFS.h>
#include <ThingSpeak.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <OneWire.h>
extern "C" {
#include "user_interface.h"
}

#define SCL 5 // D1
#define SDA 4 // D2
#define R_D 0 // D3
#define LEDPin 2 // Onboard LED

String resetReason = "restart: " + ESP.getResetReason();
String resetDetail = ESP.getResetInfo();

ESP8266WebServer server(80);
FtpServer ftpSrv;
WiFiClient client;
time_t getNtpTime();
FSInfo fs_info;
File fh,fd,fe;
Ticker secondTick;
volatile int watchDog = 0;
OneWire  ds(12);                // on GPIO12 - pin D6 on d1-mini

bool onBattery = false;          // switch to enable sleep if required
bool reboot = true;

char fileName[] = "/XXyymmdd.csv";
char fileSizeStr[10];
char todayName[] = "/XXyymmdd.csv";
char userText[] = "/XXyymmdd.csv";
uint16_t longStrLen = 10000;
char longStr[10000];
char charBuf[128];
char dateStr[7];
char timeStr[10];

uint8_t oldMin, oldQtr, oldHour, oldDay, oldMonth;
uint8_t reply[260];
float chlo1min,chlo1max,chlo1rms,chlo2min,chlo2max,chlo2rms;
float level,rain_t,rain_y=0.0,rain_m=0.0;   // rain today since midnight, yesterday, this month
float batteryVolts;
float celsius[3], sumTemp[5];
uint32_t fileSize, secsSinceRestart;
uint32_t t0, t1, bootMillis, startMillis, lastScan;
