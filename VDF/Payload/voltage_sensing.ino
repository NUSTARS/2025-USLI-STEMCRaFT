float getBatteryVoltage(void) {
  unsigned int reading = analogRead(VOLTAGE_SENSING);
  // lerp reading from 0-1023 to 0-9.9V
  return ((float)reading) * (9.9 / 1023);

}