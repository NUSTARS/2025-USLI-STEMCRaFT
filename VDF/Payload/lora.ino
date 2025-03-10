void LoRaSetup(void) {
  LoRaSerialPort.begin(115200, SWSERIAL_8N1, LORA_RX, LORA_TX, false); // if this is SoftwareSerial
  //LoRaSerialPort.begin(115200);
  delay(1500);
  LoRaSerialPort.println("AT+BAND=" + lora_band);
  delay(500);
  LoRaSerialPort.println("AT+NETWORKID=" + lora_networkid);
  delay(500);
  LoRaSerialPort.println("AT+ADDRESS=" + lora_address);
  delay(1000);
}

bool receiveStopSignal(void) {
  if (LoRaSerialPort.available() > 0) {
    String incoming_string = LoRaSerialPort.readString();
    return incoming_string.indexOf(LORA_STOP_CHAR) >= 0;
  }
}