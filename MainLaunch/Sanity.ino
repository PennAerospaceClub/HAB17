
void sanity() {
  //========================================
  //Sanity Declarations ====================
  static boolean sd = false;
  static boolean atm = false;
  static boolean gps_attached = false;
  static int rb_signal = 0;

  static boolean good_data = false;

  //==========================================
  //Sanity Checks ============================

  if (!sd) {
    if (SD.begin(chipSelect)) {
      sd = true;
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

  //ROCKBLOCK
  rb_signal = isbd.getSignalQuality();

  //========================================
  //Data Verification ======================
  if (gps_attached) {
    readGPS();
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
  if (!atm) {
    display.print("ATM- ");
  } else {
    display.print("ATM+ ");
  }

  display.println("RB: " + String(rb_signal));

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

  int current = digitalRead(BUTTON);
  display.println(current);

  String data = mkdata();
  Serial.println(data);

  //==========================================
  //Ending Sanity ============================
  if (sd && gps_attached && atm && digitalRead(BUTTON)) {
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


