bool detectLaunch(){
  Eigen::Vector3f accel = Eigen::Vector3f getLinearAcceleration();
  return accel.norm() > ACCEL_THRESHOLD; 
}

bool detectLanding(unsigned long launch_time, Eigen::Vector3f gravity){
  Eigen::Vector3f gravity = getGravity();
  Eigen::Vector3f vertical {0, 0, 1};
  if(abs(gravity.dot(vertical)) >= LAND_DETECT_THRESH){
    delay(LAND_DETECT_WAIT_TIME*1000);
    gravity = getGravity();
    return abs(gravity.dot(vertical)) >= LAND_DETECT_THRESH;
  }
  return false;
}

void sendAPRSData(float batVoltage, Eigen::Vector3f orientation, struct tm landingTime){
  std::string out = "";
  out += batVoltage + " (" + orientation(0) + ", " + orientation(1) + ", " + orientation(2) + "), " 
} 