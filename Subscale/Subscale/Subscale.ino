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
    - Add time to log
    - Look into kalman filter
    - Look into calibration
    - Add roll pitch yaw RATE

  if weird error, denied ability to access key strokes (this is for my mac specifically don't worry abt it)
*/

// ---------- Include ---------- //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO08x.h>
#include <SdFat.h> 
#include <SPI.h>

// IMU
#define BNO08X_RESET -1

// Barometer
#define SEALEVELPRESSURE_HPA (1013.25)


// IMU vars  ---------------------------------
Adafruit_BNO08x bno08x(BNO08X_RESET);
long REPORT_FREQ_HZ = 5000;

// Barometer vars ---------------------------------
Adafruit_BMP3XX bmp; // default adress set to 0x77 (I2C address)

// SD Stuff ---------------------------------------------------
SdFat SD;
FsFile dataFile;
String FILE_NAME = "data.csv";



// Structs -----------------------------------------------------
struct orientation {
  float yaw;
  float pitch;
  float roll;
};
struct acceleration {
  float xAccel;
  float yAccel;
  float zAccel;
};
struct barometerData {
  double temp;
  double press;
  double alt;
};



// Functions -------------------------------------------------------------------------
// Declaring IMU functions
int setupIMU(long reportIntervalUs);
int getIMUData(orientation* orient, acceleration* accel);
int setReports(sh2_SensorId_t reportType, long report_interval);
void printOrientation(orientation* orient);
void printAccel(acceleration* accel);

// Barometer Functions
int setupBarometer();
int getBarometerData(barometerData* baro);
void printBarometerData(barometerData* baro);

// SD Functions
int setupSD();
void logData();















