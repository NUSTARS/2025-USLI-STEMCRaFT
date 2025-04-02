void setup() {
  
  Serial.begin(115200);
  delay(800);
  Wire.begin(SDA,SCL);
  delay(800);
  setupBNOs();
  
  LoRaSetup();
  /*
  EEPROM.begin(EEPROM_SIZE*2);

  isFirstBNOCalibrated = cal_setup(bno1, 0);
  isSecondBNOCalibrated = cal_setup(bno2, 26);

  bno1.setMode(OPERATION_MODE_GYRONLY);
  bno2.setMode(OPERATION_MODE_GYRONLY);

  EEPROM.commit();
  EEPROM.end();
  */
  RFSwitcherSetup();
  voltageSensingSetup();
  
  
  debugHelper("Finished Setup", 100);
  
  state = DETECT_LANDING;
}

// main flight loop

void loop() {

  imuDataHelper();
  
  switch (state) {
    case WAIT_FOR_LAUNCH:

      // if we detect launch
      // then advance state to record launch
      if (detectLaunch()) {
        debugHelper("Launch Detected!", 400);
        start_of_state_millis = millis();
        state = FLY;
      }
      
      break;

    // ==========================================================================
    case FLY:
      if (millis() > start_of_state_millis + (TIME_UNTIL_DETECT_LANDING * 1000)) {
        state = DETECT_LANDING;
        debugHelper("Detecting Landing!", 220);
      }

      break;

    // ==========================================================================
    case DETECT_LANDING:

      // if we detect a landing,
      // advance the state      
      if (detectLanding()) {
        start_of_state_millis = millis();
        lora_helper_millis = start_of_state_millis;
        state = TRANSMIT;
        debugHelper("Transmitting!", 5000);
      }

      break;  

    // ==========================================================================
    case TRANSMIT:
      printSensorData();

      // detect which antenna is up and make RF Switch:
      gravity = getGravity();

      switchAntennaGivenGravity(gravity);

      // transmit
      //orientation = getOrientation();
      //batVoltage = getBatteryVoltage();
      //sendAPRSData(batVoltage, orientation);

      // if time is up or we receive a LoRa message
      // stop transmitting by advancing the state.
      // Probably only do this every some odd interval.
      /*
      if (millis() > start_of_state_millis + MAX_TRANSMIT_TIME * 1000) {
        state = DONE;
      }
      else if (millis() > lora_helper_millis + LORA_READ_INTERVAL * 1000) {
        lora_helper_millis += LORA_READ_INTERVAL;
        if (receiveStopSignal()) state = DONE;
      }
      */
      break;

    // ==========================================================================
    case DONE:
      // Don't do stuff :)
      // maybe buzzer
      noTone(BUZZER);
      delay(50);
      tone(BUZZER, random(300, 2000));
      delay(50);

      break;
  }
  delay(1000);
}