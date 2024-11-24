void setupSD() {
   if (!SD.begin(chipSelect)) { // Test to see if the SD card is there
    Serial.println("Card failed, or not present");
    while (1) {}
  }
  Serial.println("card initialized.");
}