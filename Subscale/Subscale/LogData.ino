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

  // Open or create a file in write mode
  dataFile = SD.open("data.txt", O_RDWR | O_CREAT | O_AT_END);

  if (dataFile && !firstTime && pullState) {
    // Write a line of data
    dataFile.printf("Temp = %.3f, Press = %.3f, Alt = %.3f, X-Accel: %.3f, Y-Accel: %.3f, Z-Accel: %.3f, Pitch = %.3f, Yaw = %.3f, Roll = %.3f \n", 
                      tap.temp, tap.press, tap.alt, yprxyz.xAccel, yprxyz.yAccel, yprxyz.zAccel, yprxyz.pitch, yprxyz.yaw, yprxyz.roll); // NEED TO ADD TIME
    dataFile.close();  // Always close the file after writing
    Serial.println("Data written to data.txt");
  } else {
    Serial.println("Failed to open file for writing.");
  }

  delay(100);  // Write data every second
}


void setupPullPin() {
  pinMode(pullPin, INPUT);
}

bool checkPullPin() {
  return digitalRead(pullPin);
}