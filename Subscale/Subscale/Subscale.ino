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
    - Download the Teensyduino library 
      - https://www.pjrc.com/teensy/tutorial.html
    - https://learn.adafruit.com/adafruit-9-dof-orientation-imu-fusion-breakout-bno085/uart-rvc-for-arduino
    - How to store data the fastest

  Things to do
    - Reset alt when on the pad (pin is removed)
    - Add time to log
    - 

  if weird error, denied ability to access key strokes (this is for my mac specifically don't worry abt it)
*/

// ---------- Include ---------- //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO08x.h>
#include <SdFat.h> 
#include <SPI.h>

// Initialize the barometer
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp; // default adress set to 0x77 (I2C address)
void barometerSetup();
void barometerLoop();
void barometerPrint();

struct BarometerData {
  double temp;
  double press;
  double alt;
} tap;

// Initalize SD card
int storeLines = 10;
int currentDataLine = 0;
String data;

SdFat SD;
FsFile dataFile;

unsigned long previousMillis = 0; // Stores the last log time
const unsigned long dataLogInterval = 50; 
unsigned long currentTime = 0;

// Initialize Pull Pin
const int pullPin = 5;
bool firstTime = true;

// Initialize IMU
#define BNO08X_RESET -1

struct euler_t {
  float yaw;
  float pitch;
  float roll;
  float xAccel;
  float yAccel;
  float zAccel;
} yprxyz;

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
long reportIntervalUs = 5000;

// Declaring functions
void setupIMU();
void setReports(sh2_SensorId_t reportType, long report_interval);
void quaternionToEuler();
void quaternionToEulerRV();
void IMULoop();
void IMUPrint();

void setupSD();
void logData();

void setupPullPin();
bool checkPullPin();















