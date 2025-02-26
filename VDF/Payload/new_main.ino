// main flight loop

void loop() {
  switch (state) {
    case INIT:

      state = CALIBRATE; // directly flow into CALIBRATE
      start_of_state_milis = milis();

    case CALIBRATE:

      // if BNOs are calibrated or we have waited
      // longer than MAX_CALIB_TIME, then advance state
      // to WAIT_FOR_LAUNCH

      /*
      if ((milis() > start_of_state_milis + MAX_CALIB_TIME) | 
          (**FIXME, true if calibrated**)) {
        state = WAIT_FOR_LAUNCH;
      }
      */

      break;

    case WAIT_FOR_LAUNCH:
      // if we detect launch
      // then advance state to record launch

      /*
      if (**FIXME, true if launch detected**) {
        start_of_state_milis = milis();
        state = RECORD_LAUNCH;
      }
      */
      break;

    case RECORD_LAUNCH:
      // read and write IMU data



      // once we have been recording for the
      // specified amount of time, stop recording
      // by advancing the state
      /*
      if (milis() > start_of_state_milis + EXPECTED_FLIGHT_TIME) {
        
      }
      */
      break;

    case TRANSMIT:

      // detect which antenna is up and make RF Switch

      // transmit data

      // if time is up or we receive a LoRa message
      // stop transmitting by advancing the state.
      // Probably only do this every some odd interval.
      /*
      if (FIXME) {
        state = DONE;
      }
      */
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