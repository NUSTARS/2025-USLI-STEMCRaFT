int calibration_setup(Adafruit_BNO055 &bno, uint8_t &sys, uint8_t &gyro, uint8_t &accel, uint8_t &mag) {
    int eeAddress = 0;
    long bnoID;
    EEPROM.get(eeAddress, bnoID);
    sensor_t sensor;
    bno.getSensor(&sensor);

    adafruit_bno055_offsets_t calibrationData;
    bool calibrationLoaded = false;

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

    if (!calibrationLoaded) {
        Serial.println("Starting sensor calibration...");
        while (!bno.isFullyCalibrated()) {
            bno.getCalibration(&sys, &gyro, &accel, &mag);
            delay(100);  // Delay to reduce CPU usage during calibration
        }
        Serial.println("Sensor fully calibrated.");
        EEPROM.put(eeAddress, sensor.sensor_id);                
        bno.getSensorOffsets(calibrationData);                  
        EEPROM.put(eeAddress + sizeof(long), calibrationData); 
        Serial.println("Calibration data saved to EEPROM.");
    }
}