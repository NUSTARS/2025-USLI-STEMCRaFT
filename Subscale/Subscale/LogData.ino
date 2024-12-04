int setupSD() {
  if (!SD.begin(SdioConfig(FIFO_SDIO))) { // Test to see if the SD card is there
    return -1;
  }
  return 0;
}

void logData() {
  /*
  if (firstTime) {
    Serial.println("FIRST TIME SD, CLEARING FILE");
    dataFile = SD.open("data.txt", O_WRITE | O_CREAT | O_TRUNC);
    Serial.println("SD CARD CLEARED");
    dataFile.close();

    firstTime = false;
  }

  dataFile = SD.open("data.txt", O_RDWR | O_CREAT | O_AT_END);

  if (dataFile && !firstTime) {
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
    //Serial.print(dataFile); Serial.print(firstTime);
  }
  */
}