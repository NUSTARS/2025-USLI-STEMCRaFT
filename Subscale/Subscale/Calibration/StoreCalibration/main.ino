void calibration_setup(uint8_t *sys, uint8_t *gyro, uint8_t *accel, uint8_t *mag) {
    int eeAddress = 0;
    long bnoID = 0;
    EEPROM.get(eeAddress, bnoID);
    sensor_t sensor;
    bno.getSensor(&sensor);

    adafruit_bno055_offsets_t calibrationData;
    //bool calibrationLoaded = false;

    /*
    if (bnoID == sensor.sensor_id) {
        EEPROM.get(eeAddress + sizeof(long), calibrationData);
        bno.setSensorOffsets(calibrationData);
        calibrationLoaded = true;
        Serial.println("Calibration data loaded from EEPROM.");
        bno.getSensorOffsets(calibrationData);
    } else {
        calibrationLoaded = false;
        Serial.println("No calibration data found in EEPROM.");
    }
    */

  Serial.println("Starting sensor calibration...");
  /*
  while (!bno.isFullyCalibrated()) {
    bno.getCalibration(sys, gyro, accel, mag);
    delay(100);  // Delay to reduce CPU usage during calibration
  }
  */
  Serial.println("Sensor fully calibrated.");
  EEPROM.put(eeAddress, sensor.sensor_id);                
  bno.getSensorOffsets(calibrationData);                  
  EEPROM.put(eeAddress + sizeof(long), calibrationData); 
  Serial.println("Calibration data saved to EEPROM.");

}

void displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    //Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }

    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.println(mag, DEC);
}

void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.println(calibData.mag_radius);
}

void loop() {

  sensors_event_t orientationData, angVelocityData, linearAccelData;

  getIMUData(&orientationData, &angVelocityData, &linearAccelData);

    uint8_t sys, gyro, accel, mag = 0;

/*

    bno.getCalibration(&sys, &gyro, &accel, &mag);
    
    Serial.print(F("Calibration: "));
    Serial.print(sys, DEC);
    Serial.print(F(", "));
    Serial.print(gyro, DEC);
    Serial.print(F(", "));
    Serial.print(accel, DEC);
    Serial.print(F(", "));
    Serial.print(mag, DEC);
    Serial.println(F(""));
    Serial.println(bno.isFullyCalibrated());
    
    delay(BNO055_SAMPLERATE_DELAY_MS);

*/

    displayCalStatus();
    adafruit_bno055_offsets_t calibrationData;
    bno.getSensorOffsets(calibrationData); 
    displaySensorOffsets(calibrationData);

    if (bno.isFullyCalibrated()) {
      calibrated = true;
      Serial.println("ROCKET CALIBRATED");
      calibration_setup(&sys, &gyro, &accel, &mag);
      while(1);
    }
}
