

bool stop_signal_received?() {
  if (LoRaSerialPort.available() > 0) {
    incomingString = LoRaSerialPort.readString();
    return incomingString.index_of(LORA_STOP_CHAR) >= 0;
  }
}