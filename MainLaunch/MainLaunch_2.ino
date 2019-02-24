/*
   Main Flight Code for the Penn Aerospace Club's
   High Altitude Balloon Project

   Integrations:
   SD Card, 9DOF IMU, Temperature, Humidity,
   GPS, Geiger Counter Logging, Altitude Control,
   Comprehensive Sanity System, Anemometer

*/
//======================================
// Libraries ===========================
#include <Wire.h>               //Temp/Humidity/IMU/OLED
#include <SPI.h>                //SD
#include <SD.h>                 //SD
#include <TinyGPS++.h>          //GPS
#include <Adafruit_SSD1306.h>   //OLED
#include <Adafruit_HTU21DF.h>   //Temp/Humidity
#include <Adafruit_Sensor.h>    //IMU
#include <Adafruit_BNO055.h>
#include <Adafruit_MPL3115A2.h>  //barometer
//#include <Adafruit_SI1145.h> //UV
//#include <Adafruit_LSM303_U.h>  //IMU
//#include <Adafruit_L3GD20_U.h>  //IMU
//#include <Adafruit_9DOF.h>      //IMU
#include <IridiumSBD.h>         //ROCKBLOCK
#include <SoftwareSerial.h>     //ROCKBLOCK

//=====================================
//IMU =================================
//Adafruit_9DOF                   dof   = Adafruit_9DOF();
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

//UV =================================
//Adafruit_SI1145 uv = Adafruit_SI1145();

//pressure ============================
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

//IMU variables =========================
float eulerx;
float eulery;
float eulerz;

//pressure variables ====================
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

//Temp/Humidity ============================
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
float temp;                 //farenehit
float humidity;             //percentage

//Anemometer ===============================
#define ANEM A0
float windspeed;            //mph

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

//Solar Panel ===============================
#define SOLAR A7
float voltage;

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

  //solar panel
  pinMode(SOLAR, INPUT);

  pinMode(ANEM, INPUT);

  //Sanity
  pinMode(BUTTON, INPUT);

}

//=======================================
void loop() {
  if (dataSent) {
    sendRockBlockData(rbData());
  }
}

void anemometer() {
  //Takes an analog input from 0.4V and 2V and maps it to 0m/s to 32m/s then to mph
  windspeed = ((analogRead(ANEM) - 82) / 328.0 * 32.4) * 2.23694;
}

void solarPanel() {
  voltage = analogRead(SOLAR) * (5.0 / 1023.0);
}


