

void loop() {


  sensors_event_t orientationData, angVelocityData, linearAccelData;
  barometerData baro;
  float magnitude;
  float altitude_offset = 0.0;

  getBarometerData(&baro, altitude_offset);
  delay(BNO055_SAMPLERATE_DELAY_MS);
  getBarometerData(&baro, altitude_offset);
  Serial.print(altitude_offset);
  altitude_offset = (&baro)->alt; 
  Serial.print(altitude_offset);
  printBarometerData(&baro);


  // STARTUP WAITING
  do {
    getIMUData(&orientationData, &angVelocityData, &linearAccelData);
    getBarometerData(&baro, altitude_offset);


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
    printEvent(&linearAccelData);
    printBarometerData(&baro);
  } while (magnitude < THRESH_ACCEL);
  Serial.println("LAUNCHED!");
  tone(BUZZER, 1500);


  // LOGGING
  data dataArr[2*LOG_TIME * LOG_FREQ];
  int currentPoint = 0;
  unsigned long loggingStartTime = millis();  // Capture start time



  while (!openFlaps(&linearAccelData, &baro)) {
    unsigned long timeStarted = millis();
    Serial.println("Burnout not reached.");
    getIMUData(&orientationData, &angVelocityData, &linearAccelData);
    getBarometerData(&baro, altitude_offset);
    printEvent(&linearAccelData);
    // printEvent(&orientationData);
    // printEvent(&angVelocityData);
    printBarometerData(&baro);


    dataArr[currentPoint].pressure = baro.press;
    dataArr[currentPoint].temp = baro.temp;
    dataArr[currentPoint].altitude = baro.alt;

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

  Serial.println("Burnout reached!!.");
  tone(BUZZER, 600);

  for (int i = 0; i < LOG_TIME * LOG_FREQ; i++) {  // 6000 originally, making it less for testing
    unsigned long timeStarted = millis();

    getIMUData(&orientationData, &angVelocityData, &linearAccelData);
    getBarometerData(&baro, altitude_offset);

    // CHANGING CURERNT POINT TO i
    dataArr[currentPoint].pressure = baro.press;
    dataArr[currentPoint].temp = baro.temp;
    dataArr[currentPoint].altitude = baro.alt;

    dataArr[currentPoint].euler_x = orientationData.orientation.x;
    dataArr[currentPoint].euler_y = orientationData.orientation.y;
    dataArr[currentPoint].euler_z = orientationData.orientation.z;

    dataArr[currentPoint].accel_x = linearAccelData.acceleration.x;
    dataArr[currentPoint].accel_y = linearAccelData.acceleration.y;
    dataArr[currentPoint].accel_z = linearAccelData.acceleration.z;

    dataArr[currentPoint].ang_x = angVelocityData.gyro.x;
    dataArr[currentPoint].ang_y = angVelocityData.gyro.y;
    dataArr[currentPoint].ang_z = angVelocityData.gyro.z;

    // printEvent(&orientationData);
    // printEvent(&angVelocityData);
    // printEvent(&linearAccelData);
    // printBarometerData(&baro);

    while (millis() - timeStarted < 1000.0 / LOG_FREQ) {}

    dataArr[currentPoint].time = millis();

    //unsigned long loggingStartTime = millis();  // Capture start time
    logData2(dataArr);
    //Serial.println(millis() - loggingStartTime);


    //Serial.println(dataArr[currentPoint].time); //TESTING
  }

  //SD WRITE

  Serial.println("DONE LOGGING");
  actuationServo.write(SERVO_MIN_ANGLE);


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
