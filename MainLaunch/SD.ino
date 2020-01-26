void writeSD(String dataString){
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("PRINTED TO SD");
  }
}

String mkdata(){
  String data = 
  //Times
  String(gpstime) + "," + 
  String(gpsdate) + "," + 
  String(millis()) + "," + 
  //GPS
  String(latit, 6) + "," + 
  String(longit, 6) + "," + 
  String(alt, 6) + "," + 
  String(course, 2) + "," + 
  String(speed, 2) + "," + 
  String(numsats) + "," + 
  String(velDiff, 3) + "," + 
  //ATM data
  String(pascals, 4) + "," +
  String(humidity, 2) + "," +
  String(gas, 2) + "," +
  String(altm, 2) + "," +
  String(tempC, 2) + "," +
  // RockBlock
  String(nichromeOn, 1);
  
  return data;
}

String rbData(){
  String data = 
  String(latit, 6) + "," + 
  String(longit, 6) + "," + 
  String(altm, 6) + "," +  
  String(speed, 2) + "," +
  String(tempC, 2) + "," +
  String(nichromeOn, 1);
  
  return data;
}

