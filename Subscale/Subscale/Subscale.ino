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
    - Check why we are getting random data into SD card
    - Make sure to validate which axis is up
    - Timer for getting all data points at a constant rate
    - Look into calibration

  if weird error, denied ability to access key strokes (this is for my mac specifically don't worry abt it)
*/

// ---------- Include ---------- //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
#include <SdFat.h> 
#include <SPI.h>

#define BUZZER 12

#define LOG_FREQ 100 // in Hz
#define LOG_TIME 1 // in s (CHANGE THIS BACK) to 60
#define THRESH_ACCEL -10 // in ft/s^2 (CHANGE THIS BACK) to 10
#define FILE_NAME "data.csv" // CHANGING THIS TO A TEXT FILE BC GETTING REALLY GOOFY NUMBERS IN CSV

// Barometer
#define SEALEVELPRESSURE_HPA (1013.25)


// IMU vars  ---------------------------------
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

// Barometer vars ---------------------------------
Adafruit_BMP3XX bmp; // default adress set to 0x77 (I2C address)

// SD Stuff ---------------------------------------------------
SdFat SD;
FsFile dataFile;
int linspace; // fixes printing for imu to make it easier to see 



// Structs -----------------------------------------------------
struct barometerData {
  float temp;
  float press;
  float alt;
};

struct data {
  float time;
  float temp;
  float pressure;
  float altitude;
  float euler_x;
  float euler_y;
  float euler_z;
  float ang_x;
  float ang_y;
  float ang_z;
  float accel_x;
  float accel_y;
  float accel_z;
};

// Functions -------------------------------------------------------------------------
// Declaring IMU functions
int getIMUData(sensors_event_t* orientationData, sensors_event_t* angVelocityData, sensors_event_t* linearAccelData);
void printEvent(sensors_event_t* event);

// Barometer Functions
int setupBarometer();
int getBarometerData(barometerData* baro);
void printBarometerData(barometerData* baro);

// SD Functions
int setupSD();
void logData(data* dataArr, int arrLen);















