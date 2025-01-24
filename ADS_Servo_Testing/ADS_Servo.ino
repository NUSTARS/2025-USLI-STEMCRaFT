#include <Servo.h>
// add any other libraries needed

double desiredAreaPercent; //double from 0 to 1 given by funciton f from SPARC
const int SERVO_MIN_ANGLE = 90; //min servo angle corresponding to 0% actuation, right now is arbitrary 
const int SERVO_MAX_ANGLE = 150; //max servo angle corresponding to 100% actuation, right now is arbitrary 
int servoAngle;  
Servo actuationServo; 

double i = 0; // will be used for testing servo until calculation function complete

/**
 * Converts the desired servo rotation percent to an actual servo positions
 * @param servoActuationPercent is the desired servo rotation percent
 * @return the servo angle as an integer
*/
int PercentToAngle(double servoActuationPercent){
    return round((SERVO_MIN_ANGLE + servoActuationPercent*((SERVO_MAX_ANGLE)-(SERVO_MIN_ANGLE))));
}

void setup() {
    Serial.begin(115200);
    actuationServo.attach(4); //replace with actual pin
}

void loop() {
    desiredAreaPercent = i; //will be a function that outputs percent of area needed to cover (use incrementing function for testing)
    servoAngle = PercentToAngle(i);
    actuationServo.write(servoAngle); 

    Serial.print("Desired Percent: ");
    Serial.print(desiredAreaPercent);
    Serial.print(" -> Servo Angle: ");
    Serial.println(servoAngle);

    i += 0.1;
    if(i >= 1){
      i = 0;
    }
    delay(500);
}
