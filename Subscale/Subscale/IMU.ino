 void setupIMU() {
  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  setReports(reportType, reportIntervalUs);
 }

void setReports(sh2_SensorId_t reportType, long report_interval) {
  if (! bno08x.enableReport(reportType, report_interval)) {
    Serial.println("Could not enable stabilized remote vector");
  }
}

 void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* yprxyz, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    yprxyz->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    yprxyz->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    yprxyz->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      yprxyz->yaw *= RAD_TO_DEG;
      yprxyz->pitch *= RAD_TO_DEG;
      yprxyz->roll *= RAD_TO_DEG;
    }
}

void quaternionToEulerGI(sh2_GyroIntegratedRV_t* rotational_vector, euler_t* yprxyz, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, yprxyz, degrees);
}

void IMULoop() {
    if (bno08x.getSensorEvent(&sensorValue)) {
      quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &yprxyz, true);
    }
    setReports(reportType, reportIntervalUs);

  sensors_event_t event;
  yprxyz.xAccel = event.acceleration.x;
  yprxyz.yAccel = event.acceleration.y;
  yprxyz.zAccel = event.acceleration.z;


}

void IMUPrint() {
    Serial.print(yprxyz.yaw);
    Serial.print(yprxyz.pitch);
    Serial.print(yprxyz.roll);
}









