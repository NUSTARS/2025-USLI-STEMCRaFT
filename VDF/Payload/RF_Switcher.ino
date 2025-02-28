#define RS_0 = 19;
#define RS_1 = 20;
#define RS_2 = 21;

void SwitchAntenna(int num) {
  switch (num) {
    case 1:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 2:
      digitalWrite(RS_0, HIGH);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 3:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, HIGH);
      digitalWrite(RS_2, LOW);
    break
  }

}