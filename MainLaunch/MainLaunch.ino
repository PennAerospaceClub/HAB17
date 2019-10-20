/*
   Main Flight Code for the Penn Aerospace Club's
   High Altitude Balloon Project

   Integrations:
   SD Card, temperature, humidity, barometric 
   pressure and VOC gas, Comprehensive Sanity System

*/
//======================================
// Libraries ===========================
#include <Wire.h>               //OLED
#include <SPI.h>                //SD/ATM
#include <SD.h>                 //SD
#include <TinyGPS++.h>          //GPS
#include <Adafruit_SSD1306.h>   //OLED
#include <Adafruit_Sensor.h>    //ATM
#include <Adafruit_BME680.h>    //ATM
#include <IridiumSBD.h>         //ROCKBLOCK
#include <SoftwareSerial.h>     //ROCKBLOCK

//=====================================
//ATM =================================
#define SEALEVELPRESSURE_HPA (1017.5)
//#define BME_SCK 40
//#define BME_MISO 41
//#define BME_MOSI 42
//#define BME_CS 43
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);
Adafruit_BME680 bme(53); // Hardware SPI

//ATM variables =======================
float pascals;
float altm;
float humidity;
float gas;
float tempC;

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


