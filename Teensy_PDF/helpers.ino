bool detectLaunch() {
  imuDataHelper();
  Eigen::Vector3f accel = MPSS_TO_FTPSS * (Eigen::Vector3f) getLinearAcceleration();
  // Serial.print("Accel: ");
  // Serial.println(accel.norm());
  // TODO: Switch to ft/s^2
  if (accel.norm()  > ERROR_ACCEL_THRESHOLD) return false;
  return accel.norm() > ACCEL_THRESHOLD; 
}

bool detectLanding() {
  imuDataHelper();
  Eigen::Vector3f gravity = getGravity();
  Serial.print(gravity(0)/9.81);
  Serial.print(", ");
  Serial.print(gravity(1)/9.81);
  Serial.print(", ");
  Serial.println(gravity(2)/9.81);
  delay(BNO055_SAMPLERATE_DELAY_MS);
  
  Eigen::Vector3f vertical {1, 0, 0};
  if(abs(gravity.dot(vertical)) <= LAND_DETECT_THRESH){
    delay(LAND_DETECT_WAIT_TIME*1000);
    imuDataHelper();
    gravity = getGravity();
    return abs(gravity.dot(vertical)) <= LAND_DETECT_THRESH;
  }
  return false;
}


void sendAPRSData(float batVoltage, Eigen::Vector3f orientation, String time){
  
  //std::string out = String(batVoltage) + " (" + String(orientation(0)) + ", " + String(orientation(1)) + ", " + String(orientation(2)) + "), ";
  char cstr[62];
  std::sprintf(cstr, "KK7YNC,Vbat:%.2f,Y:%.2f,P:%.2f,R:%.2f,T:%s,KK7YNC", batVoltage, orientation(0), orientation(1), orientation(2), time.c_str());
  
  size_t size = sizeof(cstr) / sizeof(char);
  uint8_t buf[size];

  Serial.println(cstr);
  Serial.println(size);

  for (size_t i = 0; i < size; i++) 
  {
    buf[i] = (uint8_t) cstr[i]; 
  }

  //const uint8_t* const_data = (const uint8_t*) buf; 

  Wire.beginTransmission(WORKER_ADDR);

  // std::string test(cstr);
  Wire.write(buf, size);
  //Wire.write(65);
  Wire.endTransmission();
}

void debugHelper(String state, unsigned int toneF) {
  Serial.println("State: " + state);
  tone(BUZZER, toneF, 1000);
}