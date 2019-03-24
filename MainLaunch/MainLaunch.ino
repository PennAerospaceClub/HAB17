/*
   Main Flight Code for the Penn Aerospace Club's
   High Altitude Balloon Project

   Integrations:
   SD Card, 9DOF IMU, Temperature, Humidity,
   GPS, Geiger Counter Logging, Altitude Control,
   Comprehensive Sanity System

*/
//======================================
// Libraries ===========================
#include <Wire.h>               //Temp/Humidity/IMU/OLED
#include <SPI.h>                //SD/ATM
#include <SD.h>                 //SD
#include <TinyGPS++.h>          //GPS
#include <Adafruit_SSD1306.h>   //OLED
#include <Adafruit_Sensor.h>    //IMU/ATM
#include <Adafruit_BNO055.h>    //IMU
#include <Adafruit_BMP3XX.h>    //ATM
#include <IridiumSBD.h>         //ROCKBLOCK
#include <SoftwareSerial.h>     //ROCKBLOCK

//=====================================

//IMU =================================
//Adafruit_9DOF                   dof   = Adafruit_9DOF();
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

//UV =================================
//Adafruit_SI1145 uv = Adafruit_SI1145();

//ATM ============================
#define SEALEVELPRESSURE_HPA (1018.3)
Adafruit_BMP3XX bmp(53);


//IMU variables =========================
float eulerx;
float eulery;
float eulerz;

//ATM variables ====================
float pascals;
float altm;
float tempC;

//UV variables ==========================

float infrared;
float ultraviolet;

//=======================================
// GPS Variables ========================
TinyGPSPlus gps;
double latit;   //degrees
double longit;  //degrees
double alt;     //meters
double alto;
double velDiff;
double vel[] = {0, 0, 0, 0, 0}; //meters
long gpstime;   //HHMMSSCC
long gpsdate;   //DDMMYY
int numsats;
double course;  //degrees
double speed;   //mph

//OLED =====================================
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//SD Card ==================================
#define chipSelect 53

//ROCKBLOCK ===============================
#define SET 7
#define UNSET 8
long nichromeTimer;
bool nichromeOn = false;
int sleepPin = 3;
uint8_t buffer[270];
size_t bufferSize = 0;
SoftwareSerial nss(12, 11); // RX, TX
IridiumSBD isbd(nss, sleepPin);
bool dataSent = true;
int messagesReceived = 0;

//Sanity ===================================
boolean sane = false;
#define BUTTON 5
//==========================================

void setup() {
  //Serial
  Serial.begin(115200); //Computer
  Serial1.begin(9600); //GPS

  //OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);

  //ROCKBLOCK
  nss.begin(19200);
  int signalQuality = -1;
  isbd.attachConsole(Serial);
  isbd.attachDiags(Serial);
  Serial.println("Step 1 done");
  isbd.setPowerProfile(0);
  Serial.println("Step 2 done");
  pinMode(sleepPin, OUTPUT);
  digitalWrite(sleepPin, LOW);
  isbd.begin();
  Serial.println("Step 3 done");
  //set/unset
  pinMode(SET, OUTPUT);
  digitalWrite(SET, LOW);
  pinMode(UNSET, OUTPUT);
  digitalWrite(UNSET, LOW);


  //take a look at the code below for reference of checking signal quality
  int err = isbd.getSignalQuality(signalQuality);
  if (err != 0) {
      Serial.print("SignalQuality failed: error ");
      Serial.println(err);
      return;
   }

  //Sanity
  pinMode(BUTTON, INPUT);

}

//=======================================
void loop() {
  if (dataSent) {
    sendRockBlockData(rbData());
  }
}


