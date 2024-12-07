void loop()  {
  
  sensors_event_t orientationData , angVelocityData , linearAccelData;
  barometerData baro;

  // STARTUP WAITING
  do{
  getIMUData(&orientationData, &angVelocityData, &linearAccelData);
  } while (linearAccelData.acceleration.y < THRESH_ACCEL);

  // LOGGING
  data dataArr[LOG_TIME * LOG_FREQ];
  int currentPoint = 0;

  for(int i = 0; i < 60000; i++){

    long timeStarted = millis();

    getIMUData(&orientationData, &angVelocityData, &linearAccelData);
    getBarometerData(&baro);

    dataArr[currentPoint].pressure = baro.press;
    dataArr[currentPoint].temp = baro.temp;
    dataArr[currentPoint].altitude = baro.alt;

    dataArr[currentPoint].euler_x = orientationData.orientation.x;
    dataArr[currentPoint].euler_y = orientationData.orientation.y;
    dataArr[currentPoint].euler_z = orientationData.orientation.z;

    dataArr[currentPoint].accel_x = linearAccelData.acceleration.x;
    dataArr[currentPoint].accel_y = linearAccelData.acceleration.y;
    dataArr[currentPoint].accel_z = linearAccelData.acceleration.z;

    dataArr[currentPoint].ang_x = linearAccelData.gyro.x;
    dataArr[currentPoint].ang_y = linearAccelData.gyro.y;
    dataArr[currentPoint].ang_z = linearAccelData.gyro.z;

    while(millis() - timeStarted < 1000.0/LOG_FREQ){}

    dataArr[currentPoint].time = millis();

  }

  //SD WRITE
  logData(dataArr, LOG_TIME*LOG_FREQ);

  // END
  noTone(BUZZER);
  while(1);
  
}

  //printEvent(&orientationData);
  //printEvent(&angVelocityData);
  //printEvent(&linearAccelData);

  //printBarometerData(&baro);

  //Serial.println(timeToLog);
  //logData();


