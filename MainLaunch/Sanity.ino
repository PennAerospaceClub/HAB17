
void sanity() {
  //========================================
  //Sanity Declarations ====================
  static boolean sd = false;
  static boolean atm = false;
  static boolean gps_attached = false;
  static boolean lux_attached = false;
  static int rb_signal = 0;

  static boolean good_data = false;

  //==========================================
  //Sanity Checks ============================

  if (!sd) {
    if (SD.begin(chipSelect)) {
      sd = true;
    }
  }

  if (!lux_attached) {
    if (initLux()) {
      lux_attached = true;
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
//  rb_signal = isbd.getSignalQuality();

  //========================================
  //Data Verification ======================
  if (gps_attached) {
    readGPS();
  }
  if (atm) {
    readATM();
  }

  if(lux_attached) {
    readLux();
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

  if (!lux_attached) {
    display.print("LUX- ");
  } else {
    display.print("LUX+ ");
  }
  //display.println("RB: " + String(rb_signal));

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
    display.println();
    display.println(String(altm, 2) + "m");
  }

  //int current = digitalRead(BUTTON);
  //display.println(current);
  if (lux_attached) {
    display.println(String(lux, 4));
  }

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


