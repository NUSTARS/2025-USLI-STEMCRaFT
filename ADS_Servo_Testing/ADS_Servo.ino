#include <Servo.h>
// add any other libraries needed for SPARC

double desiredAreaPercent; //double from 0 to 1 given by funciton f from SPARC
int servoAngle;  
Servo mainServo; 

double i = 0.1; // will be used for testing servo until SPARC function complete

int areaToAngleConversion(double flapArea){
    
}


void setup() {
    Serial.begin(115200);
    mainServo.attach(4); //replace with actual pin
}

void loop() {
    desiredAreaPercent = i //will be a function that outputs percent of area needed to cover (use incrementing function for testing)
    servoAngle = areaToAngleConversion(desiredAreaPercent);
    mainServo.write(servoAngle); 
}
