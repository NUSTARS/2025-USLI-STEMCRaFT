void setup() {
  Serial.begin(115200);

  delay(1000);

  if(!bno.begin()){
    Serial.println("BNO Failed");
    tone(BUZZER, 200);
    while(1);
  }
  if(setupBarometer() != 0){
    Serial.println("BMP Failed");
    tone(BUZZER, 200);
    while(1);
  }

  setupSD();

  Wire.setClock(400000UL);

  pinMode(BUZZER, OUTPUT);

  /*
  for(int i = 200; i < 1500; i++){
    tone(BUZZER, i);
    delay(1);
  }
  for(int i = 1500; i > 200; i--){
    tone(BUZZER, i);
    delay(1);
  }
  */
  tone(BUZZER, 450); // CHANGING THIS FROM 1000 DOWN TO 450 BC ANNOYING WHEN TESTING

  // TESTING DOWN HERE FOR PRINT STATEMENTS
  linspace = 0;

}