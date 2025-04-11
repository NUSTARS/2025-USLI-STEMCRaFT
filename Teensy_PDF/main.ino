void setup() {
  Serial.begin(115200);
  delay(800);
  Wire.begin();
  //Wire.begin(SDA2,SCL2);
  //Wire1.begin(SDA,SCL,100000);

  delay(800);
  setupBNOs();
  
  LoRaSetup();


  

  // EEPROM.begin(EEPROM_SIZE*2);

  // isFirstBNOCalibrated = cal_setup(bno1, 0);
  // isSecondBNOCalibrated = cal_setup(bno2, 26);

  // bno1.setMode(OPERATION_MODE_GYRONLY);
  // bno2.setMode(OPERATION_MODE_GYRONLY);

  // EEPROM.commit();
  // EEPROM.end();

  RFSwitcherSetup();
  voltageSensingSetup();
  lora_sent_time = 0;
  
  debugHelper("Finished Setup", 100);
  
  state = WAIT_FOR_LAUNCH;
}

// main flight loop

void loop() {

  //imuDataHelper();
  
  switch (state) {
    case WAIT_FOR_LAUNCH:

      if (lora_sent_time == 0) {
        lora_sent_time = receiveTime();
      }

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
        helper1 = start_of_state_millis;
        helper_bool = true;
        lora_helper_millis = start_of_state_millis;
        state = TRANSMIT;


        // ADDING CODE TO CALCULATE TIME 
        landing_time = lora_sent_time + millis() - recieved_time;

        // CONVERT LANDING TIME TO ACTUAL MILITARY TIME 
        unsigned long hour = landing_time / 3600000;
        unsigned long min = (int) ((landing_time % 3600000) / 60000);
        unsigned long seconds = (int) (((landing_time % 3600000) / 60000.0 - min) * 60);

        Serial.println(hour);
        Serial.println(min);
        Serial.println(seconds);

        transmitted_landing_time = String(hour) + ":" + String(min) + ":" + String(seconds);

        debugHelper("Transmitting!", 5000);
      }
      
      

      break;  

    // ==========================================================================
    case TRANSMIT:
      //printSensorData();
      // detect which antenna is up and make RF Switch:
      
      
      //
      if (millis() > helper1 + 5000) {
        helper1 = millis();
        Serial.println("hi");

        if (helper_bool) {
          
          imuDataHelper();
          orientation = getOrientation();
          batVoltage = getBatteryVoltage();
          gravity = getGravity();
          switchAntennaGivenGravity(gravity);
        }
        else {
          if (gravity(0) != -1000000) {
            // ADD IN VARIABLE FOR TIME 
            sendAPRSData(batVoltage, orientation, transmitted_landing_time);
          }
        }
        helper_bool = !helper_bool;
      }
    

      
      // transmit
      

      //delay(100);

      // if time is up or we receive a LoRa message
      // stop transmitting by advancing the state.
      // Probably only do this every some odd interval.
      
      // if (millis() > start_of_state_millis + MAX_TRANSMIT_TIME * 1000) {
        // state = DONE;
        // Serial.println("DONE!!!!");
      // }
      // else 
      if (millis() > lora_helper_millis + LORA_READ_INTERVAL * 1000) {
        lora_helper_millis += LORA_READ_INTERVAL * 1000;
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
  //delay(500);
}
