int setupSD() {
  if (!SD.begin(SdioConfig(FIFO_SDIO))) { // Test to see if the SD card is there
    return -1;
  }
  dataFile = SD.open(FILE_NAME, O_WRITE | O_CREAT | O_TRUNC); // clear the data file not necessary for real code as long as we clear or remove the file
  return 0;
}







void logData(data* dataArr, int arrLen) {
  dataFile = SD.open(FILE_NAME, O_RDWR | O_CREAT | O_AT_END);

  
  //dataFile.write("-----time-temp-pressure-altitude-euler_x-euler_y-euler_z-ang_x-ang_y-ang_z-accel_x-accel_y-accel_z-----\n");



  for(int i = 0; i < arrLen; i++){
    String line = "";
    line += String(dataArr[i].time) + ",";


    
    line += String(dataArr[i].temp) + ",";
    
    line += String(dataArr[i].pressure) + ",";
    line += String(dataArr[i].altitude) + ",";
    line += String(dataArr[i].euler_x) + ",";
    line += String(dataArr[i].euler_y) + ",";
    line += String(dataArr[i].euler_z) + ",";
    line += String(dataArr[i].ang_x) + ",";
    line += String(dataArr[i].ang_y) + ",";
    line += String(dataArr[i].ang_z) + ",";
    line += String(dataArr[i].accel_x) + ",";
    line += String(dataArr[i].accel_y) + ",";
    line += String(dataArr[i].accel_z) + "\n";
    
    
    Serial.println(line);
    dataFile.write(line.c_str());
  }
  dataFile.close();  
}

