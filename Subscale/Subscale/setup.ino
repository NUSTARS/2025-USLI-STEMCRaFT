void setup() {
  Serial.begin(115200);

  barometerSetup();
  setupIMU();
  setupSD();
  setupPullPin();
}