void barometerSetup() {
    // Setup for barometer
  while (!Serial);
  Serial.println("Adafruit BMP388 / BMP390 test");

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_16X); // Can change these oversampling rates by powers of 2 from none to 16 (higher means more precise but less speed)
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_15); // Smooths sensor outputs with the options: BMP3_IIR_FILTER_DISABLE, BMP3_IIR_FILTER_COEFF_1, BMP3_IIR_FILTER_COEFF_3, BMP3_IIR_FILTER_COEFF_7, BMP3_IIR_FILTER_COEFF_15, BMP3_IIR_FILTER_COEFF_31.
  bmp.setOutputDataRate(BMP3_ODR_200_HZ); // determines how fast new data is made available: Options: BMP3_ODR_200_HZ, BMP3_ODR_100_HZ, BMP3_ODR_50_HZ, BMP3_ODR_25_HZ, BMP3_ODR_12_5_HZ, BMP3_ODR_6_25_HZ, etc.
  Serial.println("BMP390 READY");
}

void barometerLoop() {

  if(!bmp.performReading()) {
    Serial.println("BMP390 FAILED TO PREFORM READING");
    return;
  }

  tap.temp = bmp.temperature * 9/5 + 32; // Fahrenheit
  tap.press = bmp.pressure / 100;        // Mbar
  tap.alt = bmp.readAltitude(1013.25) * 3.28084;   // Should be in feet
}

void barometerPrint() {
  // Print data
  Serial.print("Temperature = ");
  Serial.print(tap.temp);

  Serial.print(" Pressure = ");
  Serial.print(tap.press); 

  Serial.print(" Altitude = ");
  Serial.println(tap.alt); 
}
