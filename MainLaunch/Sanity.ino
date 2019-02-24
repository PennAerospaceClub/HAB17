
void sanity() {
  //========================================
  //Sanity Declarations ====================
  static boolean sd = false;
  static boolean imu = false;
  static boolean temphum = false;
  static boolean gps_attached = false;
  static boolean pressure = false;

  static boolean good_data = false;

  //==========================================
  //Sanity Checks ============================

  if (!sd) {
    if (SD.begin(chipSelect)) {
      sd = true;
    }
  }
  if (!imu) {
    if (initSensors()) {
      imu = true;
    }
  }
  
  if(!temphum){ if (htu.begin()) { temphum = true; } }
  if(!pressure) { if (baro.begin()) { pressure = true; } }

  //GPS
  if (!gps_attached) {
    gps_attached = true;
  }

  //========================================
  //Data Verification ======================
  if (gps_attached) {
    readGPS();
  }
  if (imu) {
    readIMU();
  }
  if(pressure){ readATM(); }
  if(temphum) { readHTU(); }
  //  anemometer();

  //==========================================
  //Display Information ======================
  display.clearDisplay();
  display.setCursor(0, 0);
  if (!sd) {
    display.print("SD- ");
  } else {
    display.print("SD+ ");
  }
  if (!imu) {
    display.print("IMU- ");
  } else {
    display.print("IMU+ ");
  }
//  if (!temphum) {
//    display.print("HTU- ");
//  } else {
//    display.print("HTU+ ");
//  }
  if (!pressure) {
    display.print("ATM- ");
  } else {
    display.print("ATM+ ");
  }

  if (gps_attached) {
    display.println("NS:" + String(numsats));
    display.println("Wind: " + String(windspeed, 3));
    display.println("Lat: " + String(latit, 6));
    display.println("Long: " + String(longit, 6));
  } else {
    display.println("GPS-");
  }

  if (imu) {
    display.print(String(eulerx, 2) + ",");
    display.print(String(eulery, 2) + ",");
    display.println(String(eulerz, 2));
  }

  if (pressure) {
    display.println(pascals / 101325.01);
  }

  if (temphum) {
    display.print(String(temp) + "*F ");
    display.print(String(humidity) + "%");
    display.println();
  }

  int current = digitalRead(BUTTON);
  display.println(current);

  String data = mkdata();
  Serial.println(data);

  //==========================================
  //Ending Sanity ============================
  /*
    if(sd && imu && gps_attached && pressure && temphum){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Button Pressed\nEnding Sanity\n\nHave a Great\nFlight!");
    display.display();
    sane = true;
    delay(3000);
    display.clearDisplay();
    }
  */

  if (sd && imu && gps_attached && pressure && digitalRead(BUTTON)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Button Pressed\nEnding Sanity\n\nHave a Great\nFlight!");
    display.display();
    sane = true;
    delay(3000);
    display.clearDisplay();
  }




  display.display();

}
//==========================================


