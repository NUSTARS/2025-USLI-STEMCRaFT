void loop() {

  sensors_event_t orientationData, angVelocityData, linearAccelData;
  float magnitude;

  // STARTUP WAITING
  do {
    getIMUData(&orientationData, &angVelocityData, &linearAccelData);

    magnitude = sqrt(pow(linearAccelData.acceleration.x, 2) + pow(linearAccelData.acceleration.y, 2) + pow(linearAccelData.acceleration.z, 2));

    //calibration_setup(bno, sys, gyro, accel, mag);
    
    // Serial.print(F("Calibration: "));
    // Serial.print(sys, DEC);
    // Serial.print(F(", "));
    // Serial.print(gyro, DEC);
    // Serial.print(F(", "));
    // Serial.print(accel, DEC);
    // Serial.print(F(", "));
    // Serial.print(mag, DEC);
    // Serial.println(F(""));
    // Serial.println(calibrated);

    // printEvent(&orientationData);
    // printEvent(&angVelocityData);
    // printEvent(&linearAccelData);
    // printBarometerData(&baro);
    
    delay(BNO055_SAMPLERATE_DELAY_MS);


    Serial.print("\t\tNOT LAUNCHED YET\t\t");
    Serial.println(magnitude);
  } while (magnitude < THRESH_ACCEL);
  Serial.println("LAUNCHED!");
  tone(BUZZER, 1500);


  // LOGGING
  data dataArr[LOG_TIME * LOG_FREQ];
  int currentPoint = 0;

  for (int i = 0; i < LOG_TIME * LOG_FREQ; i++) {  // 6000 originally, making it less for testing
    unsigned long timeStarted = millis();

    getIMUData(&orientationData, &angVelocityData, &linearAccelData);

    // CHANGING CURERNT POINT TO i
    dataArr[currentPoint].pressure = 0;
    dataArr[currentPoint].temp = 0;
    dataArr[currentPoint].altitude = 0;

    dataArr[currentPoint].euler_x = orientationData.orientation.x;
    dataArr[currentPoint].euler_y = orientationData.orientation.y;
    dataArr[currentPoint].euler_z = orientationData.orientation.z;

    dataArr[currentPoint].accel_x = linearAccelData.acceleration.x;
    dataArr[currentPoint].accel_y = linearAccelData.acceleration.y;
    dataArr[currentPoint].accel_z = linearAccelData.acceleration.z;

    dataArr[currentPoint].ang_x = angVelocityData.gyro.x;
    dataArr[currentPoint].ang_y = angVelocityData.gyro.y;
    dataArr[currentPoint].ang_z = angVelocityData.gyro.z;

    while (millis() - timeStarted < 1000.0 / LOG_FREQ) {}
    dataArr[currentPoint].time = millis();
    logData2(dataArr);
  }

  //SD WRITE

  Serial.println("DONE LOGGING");

  // END

  while (1) {
    for (int i = 0; i < 3; i++) {
      noTone(BUZZER);
      delay(50);
      tone(BUZZER, random(300, 2000));
      delay(50);
    }
  }
}





//Serial.println(timeToLog);
//logData();
