boolean initATM() {
  if(!bmp.begin()){ return false; }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  return true;
}

void readATM() {
  if (bmp.performReading()) {
    tempC = bmp.temperature;
    pascals = (bmp.pressure / 100.0); //hPa
    altm = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }
  delay(10);
}
