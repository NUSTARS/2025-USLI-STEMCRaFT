void loop() {

  sensors_event_t orientationData, angVelocityData, linearAccelData;

  getIMUData(&orientationData, &angVelocityData, &linearAccelData);

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
}
