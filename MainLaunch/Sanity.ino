
void sanity() {
  //========================================
  //Sanity Declarations ====================
  static boolean sd = false;
  static boolean imu = false;
  static boolean atm = false;
  static boolean gps_attached = false;

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
  
  if (!atm) {
    if (initATM()) {
      atm = true;
    }
  }

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
  if (atm) {
    readATM();
  }

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
  if (!atm) {
    display.print("ATM- ");
  } else {
    display.print("ATM+ ");
  }

  if (gps_attached) {
    display.println("NS:" + String(numsats));
    display.println("Lat: " + String(latit, 6));
    display.println("Long: " + String(longit, 6));
  } else {
    display.println("GPS-");
  }
  if (atm) {
    //ptint stuff
    display.print(String(pascals, 2) + "hPa ");
    display.print(String(tempC, 2) + "*C ");
    display.print(String(altm, 2) + "m");
    display.println();
  }
  if (imu) {
    display.print(String(eulerx, 2) + ",");
    display.print(String(eulery, 2) + ",");
    display.println(String(eulerz, 2));
  }

  int current = digitalRead(BUTTON);
  display.println(current);

  String data = mkdata();
  Serial.println(data);

  //==========================================
  //Ending Sanity ============================
  if (sd && imu && gps_attached && atm && digitalRead(BUTTON)) {
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


