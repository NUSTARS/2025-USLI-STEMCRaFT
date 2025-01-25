void setup() {
  Serial.begin(115200);
  calibrated = false;
  delay(1000);

  if(!bno.begin(OPERATION_MODE_NDOF)){
    Serial.println("BNO Failed");
    while(1);
  }

  Wire.setClock(400000UL);

  long bnoID = 0;
  sensor_t sensor;
  bno.getSensor(&sensor);
  int eeAddress = 0;
  adafruit_bno055_offsets_t calibrationData;
  EEPROM.get(eeAddress, bnoID);

  if (bnoID == sensor.sensor_id) {
    EEPROM.get(eeAddress + sizeof(long), calibrationData);
    bno.setSensorOffsets(calibrationData);
    Serial.println("Calibration data loaded from EEPROM.");
    bno.getSensorOffsets(calibrationData);
  } else {
    Serial.println("No calibration data found in EEPROM.");
  }

  Serial.println("Setup Complete!");
}