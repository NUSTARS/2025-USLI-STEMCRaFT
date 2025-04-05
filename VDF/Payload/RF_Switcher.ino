void RFSwitcherSetup(){
  pinMode(RS_0, OUTPUT);
  pinMode(RS_1, OUTPUT);
  pinMode(RS_2, OUTPUT);
}

// SwitchAntennaGivenGravity : 
void switchAntennaGivenGravity(Eigen::Vector3f gravity) {
  float dot1 = gravity.dot(ant1);
  float dot2 = gravity.dot(ant2);
  float dot3 = gravity.dot(ant3);

  float min_dot = max(dot1, max(dot2, dot3));
  if (min_dot == dot1) {
    //switchAntenna(1);
    tone(BUZZER, 800);
    delay(200);
    noTone(BUZZER);
    delay(200);

    tone(BUZZER, 800);
    delay(200);
    noTone(BUZZER);
    delay(200);

    tone(BUZZER, 800);
    delay(200);
    noTone(BUZZER);
    delay(200);

    delay(2000);

    debugHelper("antenna1", 220);
  }
  else if (min_dot == dot2) {
    //switchAntenna(2);

    tone(BUZZER, 800);
    delay(400);
    noTone(BUZZER);
    delay(400);

    tone(BUZZER, 800);
    delay(400);
    noTone(BUZZER);
    delay(400);

    tone(BUZZER, 800);
    delay(400);
    noTone(BUZZER);
    delay(400);

    delay(2000);


    debugHelper("antenna2", 600);
  }
  else {
    //switchAntenna(3);
    tone(BUZZER, 800);
    delay(600);
    noTone(BUZZER);
    delay(600);

    tone(BUZZER, 800);
    delay(600);
    noTone(BUZZER);
    delay(600);

    tone(BUZZER, 800);
    delay(600);
    noTone(BUZZER);
    delay(600);

    debugHelper("antenna3", 1400);
  }
}

// SwitchAntenna : num -> void
// takes in a number (1-3) and switches the
// RF switcher to the associated antenna
void switchAntenna(int num) {
  switch (num) {
    case 0:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);     
    case 1:
      digitalWrite(RS_0, HIGH);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 2:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, HIGH);
      digitalWrite(RS_2, LOW);
    break;

    case 3:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, HIGH);
    break;
  }

}