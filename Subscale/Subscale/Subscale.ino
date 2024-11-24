/* Notes
  - Will be using a Teensy 4.1 to log data, that will be the primary purpose 
  - Using a BNO085 to get IMU data
    - Want angles, velocity, apogee reached, max velocity, orientation at landing
  - Using a BMP390 for alitutde calculation 
    - Want maximum altitude (apogee) (have both BNO integrate and BMP for comparisson -- 3 apogee comparissons)
  - Time of landing 
    - Will use on board clock on MCU to keep track of time 
  - Will need to log data
    - Log data in CSV format 

  Notes
    - https://www.pjrc.com/teensy/tutorial.html
    - https://learn.adafruit.com/adafruit-9-dof-orientation-imu-fusion-breakout-bno085/uart-rvc-for-arduino


  if weird error, denied ability to access key strokes 
*/

// ---------- Include ---------- //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO08x.h>
#include <SD.h> 

// Initialize the barometer
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp; // default adress set to 0x77 (I2C address)
void barometerSetup();
void barometerLoop();
void barometerPrint();

// Initalize SD card
const int chipSelect = BUILTIN_SDCARD;
unsigned long lastLogTime = 0; // Stores the last log time
const unsigned long logInterval = 1000; // Log every 1000 ms (1 second)

// Initialize IMU
#define BNO08X_RESET -1

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr;

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
long reportIntervalUs = 5000;
void setupIMU();
void setReports(sh2_SensorId_t reportType, long report_interval);
void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false);
void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false);
void IMULoop();
void IMUPrint();















