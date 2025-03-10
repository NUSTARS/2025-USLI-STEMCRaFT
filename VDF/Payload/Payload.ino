/* Notes - THESE ARE NOT CURRENTLY UPDATED :)
  - Will be using a Teensy 4.1 to log data, that will be the primary purpose 
  - Using a BNO085 to get IMU data
    - Want angles, velocity, apogee reached, max velocity, orientation at landing
  - Using a BMP390 for alitutde calculation 
    - Want maximum altitude (apogee) (have both BNO integrate and BMP for comparison -- 3 apogee comparisons)
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
#include <Adafruit_BNO055.h>
#include <SPI.h>
#include <EEPROM.h>
#include "SoftwareSerial.h"
#include <ArduinoEigenDense.h>
#include <ESP32Time.h>

// DEFINES ---------------------------------------------------------------


// Misc
#define BUZZER 5
#define VOLTAGE_SENSING 6

// RF Switcher
#define RS_1 19
#define RS_2 20
#define RS_3 21

// IMU
#define IMU_ADDRESS_A 0x28
#define IMU_ADDRESS_B 0x29


// VARIABLES TO SET

// General
Eigen::Vector3f ant1 {1, 1, 1};
Eigen::Vector3f ant2 {1, 1, 1};
Eigen::Vector3f ant3 {1, 1, 1};

#define THRESH_ACCEL 30 // in ft/s^2  (PUT TO 50)
#define MAX_FLIGHT_TIME 150 // in s
#define MAX_TRANSMIT_TIME 300 // in s
#define MAX_CALIB_TIME  20 // in s 

#define LAND_DETECT_WAIT_TIME 1 // in s
#define LAND_DETECT_THRESH 0.5 

// IMU
#define BNO055_SAMPLERATE_DELAY_MS (100)


// Lora
#define LORA_STOP_CHAR 'S'
#define LORA_TX 14
#define LORA_RX 15
#define LORA_READ_INTERVAL 10 //FIXME

const String lora_band = "865000000"; //enter band as per your country
const String lora_networkid = "5";    //enter Lora Network ID
const String lora_address = "2";      //enter Lora address
const String lora_RX_address = "1";   //enter Lora RX address (for sending)

ESP32Time rtc(-18000);  // offset in seconds GMT+1

// structs/enums
enum LaunchStates {
  WAIT_FOR_LAUNCH,
  RECORD_LAUNCH,
  TRANSMIT,
  DONE
};

// Vars --------------------------------------------------------------------------------

enum LaunchStates state = WAIT_FOR_LAUNCH;

// move these likely
sensors_event_t orientationData, angVelocityData, linearAccelData, gravity; 
float magnitude;

Adafruit_BNO055 bno1 = Adafruit_BNO055(55, IMU_ADDRESS_A, &Wire);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, IMU_ADDRESS_B, &Wire);

bool calibrated;

EspSoftwareSerial::UART LoRaSerialPort;

// Functions -------------------------------------------------------------------------

// Declaring IMU functions
int getIMUData(sensors_event_t* orientationData, sensors_event_t* angVelocityData, sensors_event_t* linearAccelData);
int getIMUGravity(sensors_event_t* gravity);
void printEvent(sensors_event_t* event);

// BNO Functions
void displayCalStatus(void);
bool cal_setup(void);

bool calibration_setup(Adafruit_BNO055& bno, uint8_t& sys, uint8_t& gyro, uint8_t& accel, uint8_t& mag);

void displaySensorStatus(void);
void displayCalStatus(void);
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);

// LoRa Functions
void LoRaSetup(void);
bool receiveStopSignal(void);

// RF Switcher Functions
void RFSwitcherSetup(void);
void switchAntennaGivenGravity(Eigen::Vector3f gravity);
void switchAntenna(int num);

// Battery Voltage
void voltageSensingSetup(void);
float getBatteryVoltage(void);

// helpers
bool detectLaunch();
bool detectLanding(unsigned long launch_time, Eigen::Vector3f gravity);
void sendAPRSData(float batVoltage, Eigen::Vector3f orientation, struct tm landingTime);
