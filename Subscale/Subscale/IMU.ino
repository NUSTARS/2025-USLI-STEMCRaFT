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
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
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

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  switch (sensorValue.sensorId) {
    case SH2_ACCELEROMETER:
    yprxyz.xAccel = sensorValue.un.accelerometer.x * 3.28084;
    yprxyz.yAccel = sensorValue.un.accelerometer.y * 3.28084;
    yprxyz.zAccel = sensorValue.un.accelerometer.z * 3.28084;
    break;

    case SH2_GYRO_INTEGRATED_RV:
    if (bno08x.getSensorEvent(&sensorValue)) {
      quaternionToEulerGI(&sensorValue.un.gyroIntegratedRV, &yprxyz, true);
    }
    break;
  }
  //IMUPrint();

}

void IMUPrint() {
  
  Serial.print("Yaw = ");
  Serial.print(yprxyz.yaw);
  Serial.print(" Pitch = ");
  Serial.print(yprxyz.pitch);
  Serial.print(" Roll = ");
  Serial.print(yprxyz.roll);
  

  Serial.print(" X-Accel = ");
  Serial.print(yprxyz.xAccel);
  Serial.print(" Y-Accel = ");
  Serial.print(yprxyz.yAccel);
  Serial.print(" Z-Accel = ");
  Serial.println(yprxyz.zAccel);

  /* Quaternion Vec
  
  Serial.print(" Real-Rotation = ");
  Serial.print(sensorValue.un.geoMagRotationVector.real);
  Serial.print(" i-Rotation = ");
  Serial.print(sensorValue.un.geoMagRotationVector.i);
  Serial.print(" j-Rotation = ");
  Serial.print(sensorValue.un.geoMagRotationVector.j);
  Serial.print(" k-Rotation = ");
  Serial.println(sensorValue.un.geoMagRotationVector.k);
  */


}









