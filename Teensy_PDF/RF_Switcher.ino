void RFSwitcherSetup(){
  pinMode(RS_0, OUTPUT);
  pinMode(RS_1, OUTPUT);
  pinMode(RS_2, OUTPUT);
}

// SwitchAntennaGivenGravity : 
void switchAntennaGivenGravity(Eigen::Vector3f gravity) {
  Serial.print("Gravity: ");
  Serial.print(gravity(0)/9.81);
  Serial.print(", ");
  Serial.print(gravity(1)/9.81);
  Serial.print(", ");
  Serial.println(gravity(2)/9.81);

  float dot1 = gravity.dot(ant1);
  float dot2 = gravity.dot(ant2);
  float dot3 = gravity.dot(ant3);

  float min_dot = min(dot1, min(dot2, dot3));
  if (min_dot == dot1) {
    switchAntenna(1);
    debugHelper("antenna1", 220);
  }
  else if (min_dot == dot2) {
    switchAntenna(2);
    debugHelper("antenna2", 600);
  }
  else {
    switchAntenna(3);
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
    break;

    case 1:
      Serial.println("a1,RS_0 HIGH");
      digitalWrite(RS_0, HIGH);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 2:
      Serial.println("a2,RS_1 HIGH");
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, HIGH);
      digitalWrite(RS_2, LOW);
    break;

    case 3:
      Serial.println("a3,RS_2 HIGH");
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, HIGH);
    break;
  }

}