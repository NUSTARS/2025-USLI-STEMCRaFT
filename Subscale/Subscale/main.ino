void loop()  {
  currentTime = millis();

  IMULoop();
  barometerLoop();
  //logData();
}


