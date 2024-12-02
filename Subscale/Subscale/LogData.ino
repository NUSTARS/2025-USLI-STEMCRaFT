void setupSD() {
  if (!SD.begin(SdioConfig(FIFO_SDIO))) { // Test to see if the SD card is there
    Serial.println("Card failed, or not present");
    while (1) {}
  }
  Serial.println("card initialized.");
}

void logData() {
  bool pullState = checkPullPin();

 

  if (pullState && firstTime) {
    Serial.println("FIRST TIME SD, CLEARING FILE");
    dataFile = SD.open("data.txt", O_WRITE | O_CREAT | O_TRUNC);
    Serial.println("SD CARD CLEARED");
    dataFile.close();

    firstTime = false;
  }

  dataFile = SD.open("data.txt", O_RDWR | O_CREAT | O_AT_END);

  if (dataFile && !firstTime && pullState) {
    if (currentDataLine < storeLines) {
      data += String(currentTime) + ", " + String(tap.temp) + ", " + String(tap.press) + ", " + String(tap.alt) + ", " + String(yprxyz.xAccel)
                               + ", " + String(yprxyz.yAccel) + ", " + String(yprxyz.zAccel) + ", " + String(yprxyz.pitch) + ", " + String(yprxyz.yaw) + ", " + String(yprxyz.roll) + "\n";
      currentDataLine++;
    }
    else {
      // Open or create a file in write mode
      //Serial.println(data.c_str());

      dataFile.write(data.c_str());
      dataFile.close();

      currentDataLine = 0;
      //Serial.println("Data written to data.txt");
    }
  } 
  else {
    //Serial.print(dataFile); Serial.print(firstTime); Serial.println(pullState); 
  }
}


// This isn't how the pull pin works. it is wired directly into the power cable so the MCU is off when its not connected
void setupPullPin() {
  pinMode(pullPin, INPUT);
}

bool checkPullPin() {
  return digitalRead(pullPin);
}