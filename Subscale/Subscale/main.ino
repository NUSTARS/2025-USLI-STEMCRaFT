void loop()  {
  
  sensors_event_t orientationData , angVelocityData , linearAccelData;
  barometerData baro;

  // STARTUP WAITING
  do{
  getIMUData(&orientationData, &angVelocityData, &linearAccelData);
  Serial.println("NOT LAUNCHED YET");
  } while (linearAccelData.acceleration.y < THRESH_ACCEL);
  Serial.println("LAUNCHED!");


  // LOGGING
  data dataArr[LOG_TIME * LOG_FREQ];
  int currentPoint = 0;
  unsigned long loggingStartTime = millis();  // Capture start time
  

  for(int i = 0; i < 1000; i++){ // 60000 originally, making it less for testing
    unsigned long timeStarted = millis();
    getIMUData(&orientationData, &angVelocityData, &linearAccelData);
    getBarometerData(&baro);

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

    dataArr[currentPoint].ang_x = linearAccelData.gyro.x;
    dataArr[currentPoint].ang_y = linearAccelData.gyro.y;
    dataArr[currentPoint].ang_z = linearAccelData.gyro.z;

    //printEvent(&orientationData);
    //printEvent(&angVelocityData);
    //printEvent(&linearAccelData);
    //printBarometerData(&baro);

    dataArr[currentPoint].time = millis() - loggingStartTime; 

    while(millis() - timeStarted < 1000.0/LOG_FREQ){}


    //Serial.println(dataArr[currentPoint].time); //TESTING



  }

  //SD WRITE
  logData(dataArr, LOG_TIME*LOG_FREQ);

  Serial.println("DONE LOGGING");


  // END
  noTone(BUZZER);
  while(1);
  
}



  

  //Serial.println(timeToLog);
  //logData();


