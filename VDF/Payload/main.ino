void setup() {
  
  Serial.begin(115200);
  setupBNOs();
  
  LoRaSetup();
  
  cal_setup(bno1, 0);
  cal_setup(bno2, 26);
  
  RFSwitcherSetup();
  voltageSensingSetup();
  
  
  debugHelper("Finished Setup", 100);
  
  
}

// main flight loop

void loop() {
  /*
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
      if (millis() > start_of_state_millis + (TIME_UNTIL_DETECT_LAUNCH * 1000)) {
        state = DETECT_LANDING;
      }

      break;

    // ==========================================================================
    case DETECT_LANDING:

      // once we have been recording for the
      // specified amount of time, stop recording
      // by advancing the state      
      if (detectLanding()) {
        start_of_state_millis = millis();
        lora_helper_millis = start_of_state_millis;
      }

      break;  

    // ==========================================================================
    case TRANSMIT:

      // detect which antenna is up and make RF Switch:
      gravity = getGravity();

      switchAntennaGivenGravity(gravity);

      // transmit
      orientation = getOrientation();
      batVoltage = getBatteryVoltage();
      sendAPRSData(batVoltage, orientation);

      // if time is up or we receive a LoRa message
      // stop transmitting by advancing the state.
      // Probably only do this every some odd interval.
      
      if (millis() > start_of_state_millis + MAX_TRANSMIT_TIME) {
        state = DONE;
      }
      else if (millis() > lora_helper_millis + LORA_READ_INTERVAL) {
        lora_helper_millis += LORA_READ_INTERVAL;
        if (receiveStopSignal()) state = DONE;
      }

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
  */
}