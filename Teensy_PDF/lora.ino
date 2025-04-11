void LoRaSetup(void) {
  //LoRaSerialPort.begin(115200, SERIAL_8N1, LORA_RX, LORA_TX);
  LoRaSerialPort.begin(115200);

  delay(1500);
  LoRaSerialPort.println("AT+BAND=" + lora_band);
  delay(500);
  LoRaSerialPort.println("AT+NETWORKID=" + lora_networkid);
  delay(500);
  LoRaSerialPort.println("AT+ADDRESS=" + lora_address);
  delay(1000);
  lora_helper_millis = 0;
}

// function for receiving the time 
unsigned long receiveTime(void) {

  // Serial.println("receiveTime");

  if (LoRaSerialPort.available() > 0) {

    // +RCV=50,7,Q123456,-99,40
    String incoming_string = LoRaSerialPort.readString();

    Serial.print("incoming_string: ");
    Serial.println(incoming_string);

    if (incoming_string.indexOf('Q') >= 0) {
      recieved_time = millis();
      int count = 0;
      for (int i = 0; i < int(incoming_string.length()); i++) {
        if (incoming_string.substring(i, i+1) == ",") {
          count++;
        }

        if (count == 3) {
          String timeStr = incoming_string.substring(i - 6, i);
          Serial.print("timeStr: ");
          Serial.println(timeStr);

          unsigned int millisTime = (timeStr.substring(0, 2).toInt())*3600000 
                        + (timeStr.substring(2, 4).toInt())*60000
                        + (timeStr.substring(4, 6).toInt())*1000;

          Serial.println(millisTime);

          return millisTime;
        }
      }
    }
  }

  return 0;
}

bool receiveStopSignal(void) {
  Serial.println("in receiveStopSignal");
  if (LoRaSerialPort.available() > 0) {
    Serial.println("serial is avaiable");
    String incoming_string = LoRaSerialPort.readString();
    Serial.println(incoming_string);
    return incoming_string.indexOf(LORA_STOP_CHAR) >= 0;
  }
  return false;
}