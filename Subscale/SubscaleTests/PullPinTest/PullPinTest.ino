/* Notes
  - The switch is closed (current flows) between terminals C and NC (outer most terminals) when the pull-pin is removed
*/

int pullPin;

void setup() {
  Serial.begin(115200);
  pullPin = 5;
  pinMode(pullPin, INPUT);

}

void loop() {
  bool pinState = digitalRead(pullPin);
  if (pinState) { Serial.println("ACTIVE"); }
  else { Serial.println("NOT ACTIVE"); }

}
