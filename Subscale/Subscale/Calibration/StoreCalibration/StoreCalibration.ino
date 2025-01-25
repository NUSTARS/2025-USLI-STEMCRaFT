// ---------- Include ---------- //
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>

#define THRESH_ACCEL 50 // in ft/s^2  (PUT TO 50)


// IMU vars  ---------------------------------
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
#define BNO055_SAMPLERATE_DELAY_MS (100)
bool calibrated;

// Structs -----------------------------------------------------

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
void displayCalStatus(void);
void calibration_setup(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag);
void displayCalStatus(void);
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);














