void loop()  {
  currentTime = millis();
  IMULoop();
  barometerLoop();
  double timeToLog = (millis()-currentTime)/1000.0;
  System.out.print("Time to Log: ")
  System.out.println(timeToLog);
  //logData();
}


