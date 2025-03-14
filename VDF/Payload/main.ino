void setup() {
  setupBNOs();
  RFSwitcherSetup();
  voltageSensingSetup();
  LoRaSetup();
  cal_setup(bno1, 0);
  cal_setup(bno2, 26);
}

// main flight loop

void loop() {
  switch (state) {
    case WAIT_FOR_LAUNCH:

      // if we detect launch
      // then advance state to record launch
      if (detectLaunch()) {
        start_of_state_millis = millis();
        state = RECORD_LAUNCH;
      }
      
      break;

    case FLY:
      // read and write IMU data

      

      break;

    case DETECT_LANDING:

      // once we have been recording for the
      // specified amount of time, stop recording
      // by advancing the state      
      if (detectLanding()) {
        start_of_state_millis = millis();
        lora_helper_millis = start_of_state_millis;
      }

      break;  

    case TRANSMIT:

      // detect which antenna is up and make RF Switch:
      Eigen::Vector3f gravity = getGravity();

      switchAntennaGivenGravity(gravity);

      Eigen::Vector3f orientation = getOrientation();
      // transmit data
      float batVoltage = getBatteryVoltage();

      sendAPRSData(batVoltage, );

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

    case DONE:
      // Don't do stuff :)
      // maybe buzzer
      noTone(BUZZER);
      delay(50);
      tone(BUZZER, random(300, 2000));
      delay(50);

      break;
  }
}