 // report interval is in Hz
 // returns -1 if setup fail, -2 if remote vector fail, -3 if accel fail, 0 if success
int setupIMU(long reportIntervalUs) {
  if (!bno08x.begin_I2C()) {
    return -1; 
  }

  return setReports(SH2_ARVR_STABILIZED_RV, reportIntervalUs);
}

// returns -2 if remote vector fail, -3 if accel fail
int setReports(sh2_SensorId_t reportType, long report_interval) {
  if (! bno08x.enableReport(reportType, report_interval)) {
    return -2;
  }
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    return -3;
  }
  return 0;
}

 // returns through orientation pointer
 // In radians by default 
 void quaternionToEuler(float qr, float qi, float qj, float qk, orientation* orient, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    orient->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    orient->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    orient->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      orient->yaw *= RAD_TO_DEG;
      orient->pitch *= RAD_TO_DEG;
      orient->roll *= RAD_TO_DEG;
    }
}

// returns through orientation 
// for gyroscope integrated orientation 
void quaternionToEulerGI(sh2_RotationVectorWAcc_t* rotational_vector, orientation* orient, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, orient, degrees);
}


// return -1 if failed, 0 if success
int getIMUData(orientation* orient, acceleration* accel) {

  sh2_SensorValue_t sensorValue;

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return -1;
  }

  switch (sensorValue.sensorId) {
    case SH2_ACCELEROMETER:
      accel->xAccel = sensorValue.un.accelerometer.x * 3.28084; // convert to ft/s^2
      accel->yAccel = sensorValue.un.accelerometer.y * 3.28084;
      accel->zAccel = sensorValue.un.accelerometer.z * 3.28084;
    break;

    case SH2_ARVR_STABILIZED_RV:
      quaternionToEulerGI(&sensorValue.un.arvrStabilizedRV, orient, true);
    break;
  }

  return 0;

}

void printOrientation(orientation* orient) {
  Serial.print("Yaw = ");
  Serial.print(orient->yaw);
  Serial.print(" Pitch = ");
  Serial.print(orient->pitch);
  Serial.print(" Roll = ");
  Serial.print(orient->roll);
}

void printAccel(acceleration* accel) {
  Serial.print(" X-Accel = ");
  Serial.print(accel->xAccel);
  Serial.print(" Y-Accel = ");
  Serial.print(accel->yAccel);
  Serial.print(" Z-Accel = ");
  Serial.println(accel->zAccel);
}







