void loop() {

  sensors_event_t orientationData, angVelocityData, linearAccelData;
  barometerData baro;
  float magnitude;
  int eeAddress = 0;
  long bnoID;
  sensor_t sensor;
  adafruit_bno055_offsets_t calibrationData;

  // STARTUP WAITING
  do {
    getIMUData(&orientationData, &angVelocityData, &linearAccelData);

    magnitude = sqrt(pow(linearAccelData.acceleration.x, 2) + pow(linearAccelData.acceleration.y, 2) + pow(linearAccelData.acceleration.z, 2));
    uint8_t sys, gyro, accel, mag = 0;

    bno.getCalibration(&sys, &gyro, &accel, &mag);
    /*
    Serial.print(F("Calibration: "));
    Serial.print(sys, DEC);
    Serial.print(F(", "));
    Serial.print(gyro, DEC);
    Serial.print(F(", "));
    Serial.print(accel, DEC);
    Serial.print(F(", "));
    Serial.print(mag, DEC);
    Serial.println(F(""));
    Serial.println(calibrated);
    */
    delay(BNO055_SAMPLERATE_DELAY_MS);
     
    
    

    if (sys == 3 && gyro == 3 && accel == 3 && mag == 3 && !calibrated) {
      calibrated = true;
      EEPROM.get(eeAddress, bnoID);
      bno.getSensor(&sensor);
      Serial.println("ROCKET CALIBRATED");
      EEPROM.put(eeAddress, sensor.sensor_id);                
      bno.getSensorOffsets(calibrationData);                  
      EEPROM.put(eeAddress + sizeof(long), calibrationData); 
      Serial.println("Calibration data saved to EEPROM.");
      for (int i = 200; i < 1500; i++) {
        tone(BUZZER, i);
        delay(1);
      }
      for (int i = 0; i < 3; i++) {
        noTone(BUZZER);
        delay(500);
        tone(BUZZER, 500);
        delay(500);
      }
    }

    if (calibrated) {
      noTone(BUZZER);
    }

    

    Serial.print("\t\tNOT LAUNCHED YET\t\t");
    Serial.println(magnitude);
  } while (magnitude < THRESH_ACCEL);
  Serial.println("LAUNCHED!");
  tone(BUZZER, 1500);


 
  while (1) {
    for (int i = 0; i < 3; i++) {
      noTone(BUZZER);
      delay(50);
      tone(BUZZER, random(300, 2000));
      delay(50);
    }
  }
}

