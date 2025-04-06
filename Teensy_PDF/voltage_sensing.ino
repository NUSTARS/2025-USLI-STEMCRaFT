void voltageSensingSetup(void) {
  pinMode(VOLTAGE_SENSING, INPUT);
}

float getBatteryVoltage(void) {
  unsigned int reading = analogRead(VOLTAGE_SENSING);
  // lerp reading from 0-4096 to 0-9.9V
  return ((float)reading) * (9.9 / 4096) * 1.05;
}