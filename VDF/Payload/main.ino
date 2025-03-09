void setup() {}

// main flight loop

void loop() {
  switch (state) {
    case INIT:

      pinMode(RS_0, OUTPUT);
      pinMode(RS_1, OUTPUT);
      pinMode(RS_2, OUTPUT);

      LoRaSetup();

      state = CALIBRATE; // directly flow into CALIBRATE
      start_of_state_millis = millis();

    case CALIBRATE:

      // if BNOs are calibrated or we have waited
      // longer than MAX_CALIB_TIME, then advance state
      // to WAIT_FOR_LAUNCH

      /*
      if ((millis() > start_of_state_millis + MAX_CALIB_TIME) | 
          (**FIXME, true if calibrated**)) {
        state = WAIT_FOR_LAUNCH;
      }
      */

      break;

    case WAIT_FOR_LAUNCH:
      // if we detect launch
      // then advance state to record launch
      getIMUData(&orientationData, &angVelocityData, &linearAccelData);

      magnitude = sqrt(pow(linearAccelData.acceleration.x, 2) + pow(linearAccelData.acceleration.y, 2) + pow(linearAccelData.acceleration.z, 2));

      delay(BNO055_SAMPLERATE_DELAY_MS);
      
      if (magnitude >= THRESH_ACCEL) {
        start_of_state_millis = millis();
        state = RECORD_LAUNCH;
      }

      break;

    case RECORD_LAUNCH:
      // read and write IMU data

      // detect landing

      
      
      // once we have been recording for the
      // specified amount of time, stop recording
      // by advancing the state
      
      if (millis() > start_of_state_millis + EXPECTED_FLIGHT_TIME) {
        start_of_state_millis = millis();
        lora_helper_millis = start_of_state_millis;
      }
      break;

    case TRANSMIT:

      // detect which antenna is up and make RF Switch:
      /*
      I WISH FOR THIS FUNCTION
      sensors_event_t gravity = getGravity(); // get gravity vector
      */
      switchAntennaGivenGravity(gravity);

      // transmit data

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