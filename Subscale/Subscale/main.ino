void loop()  {
  currentTime = millis();

  for (int i = 0; i < 10; i++) {
    IMULoop();
  }
  //barometerLoop();
  //delay(10);

  long timeToLog = (millis()-currentTime);
  Serial.println(timeToLog);
  //logData();
  
}


