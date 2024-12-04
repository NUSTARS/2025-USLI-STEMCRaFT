void setup() {

  delay(5000);

  Serial.begin(9600);

  if(setupIMU(5000) != 0){
    Serial.println("IMU Setup Failed!");
  }
  if(setupBarometer() != 0){
    Serial.println("Barometer Setup Failed!");
  }
  //setupSD();
}