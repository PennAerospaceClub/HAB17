void sendRockBlockData(String data) {
  dataSent = false;
  
  //create char buffer to send data
  char outBuffer[270];
  for (int i = 0; i < data.length(); i++) {
    outBuffer[i] = data[i];
  }

  uint8_t rxBuffer[270] = {0};
  for (int i = 0; i < data.length(); i++) {
    rxBuffer[i] = outBuffer[i];
  }

  bufferSize = sizeof(buffer);

  //Send/recieve data
  isbd.sendReceiveSBDBinary(rxBuffer, data.length(), buffer, bufferSize);

  //Recieve and print down message to Serial
  String downMessage = "";
  for (int i = 0; i < sizeof(buffer); ++i)
  {
    downMessage += (char) (buffer[i]);
  }
  
  Serial.println(downMessage);
  if (downMessage.equals("on")) {
    Serial.println("received message");
    messagesReceived += 1;
  }
  dataSent = true;
}


bool ISBDCallback()
{

  if (!sane) {
    sanity();
  } else {
    unsigned long nextloop = millis() + 1000;

    readGPS();
    readATM();

    //Data Logging
    String data = mkdata();
    writeSD(data);
    Serial.println(data);
    Serial.flush();

    while (millis() < nextloop);
  }
  return true;
}

