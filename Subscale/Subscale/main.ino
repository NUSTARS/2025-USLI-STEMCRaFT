void loop()  {
  long currentTime = millis();

  orientation currentOrient;
  acceleration currentAccel;
  barometerData baro;
  
  //getIMUData(&currentOrient, &currentAccel);
  //printOrientation(&currentOrient);
  //printAccel(&currentAccel);

  getBarometerData(&baro);
  //printBarometerData(&baro);

  long timeToLog = (millis()-currentTime);
  Serial.println(timeToLog);
  //logData();
  
}


