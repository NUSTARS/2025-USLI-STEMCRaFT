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
//#include <Servo.h>
#include <Adafruit_Sensor.h>
//#include "Adafruit_BMP3XX.h"
#include <Adafruit_BNO055.h>
//#include <SdFat.h>
#include <SPI.h>
#include <EEPROM.h>
#include "SoftwareSerial.h"

#define BUZZER 5
#define VOLTAGE_SENSING 6
//#define SERVO_PIN 24

#define RECALIB 1 // 1 = recalibrate, 0 = dont

#define LOG_FREQ 100 // in Hz
#define LOG_TIME 60 // in s (CHANGE THIS BACK) to 60
#define THRESH_ACCEL 50 // in ft/s^2  (PUT TO 50)
#define FILE_NAME "data.csv" // CHANGING THIS TO A TEXT FILE BC GETTING REALLY GOOFY NUMBERS IN CSV
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Main
enum LaunchStates {
  INIT,
  CALIBRATE,
  WAIT_FOR_LAUNCH,
  RECORD_LAUNCH,
  TRANSMIT,
  DONE
};

enum LaunchStates state = INIT;

sensors_event_t orientationData, angVelocityData, linearAccelData, gravity;
float magnitude;

unsigned long current_millis;
unsigned long start_of_state_millis;
unsigned long lora_helper_millis;

// THESE HAVE TO BE SET :)
// CAN/SHOULD these be defines?
#define LORA_READ_INTERVAL 10 //FIXME
#define MAX_CALIB_TIME = 20 // FIXME
#define EXPECTED_FLIGHT_TIME 30 // FIXME
#define MAX_TRANSMIT_TIME 40 // FIXME

// RF Switcher
#define RS_0 19
#define RS_1 20
#define RS_2 21

// Barometer
#define SEALEVELPRESSURE_HPA (1013.25)

// IMU vars  ---------------------------------

#define IMU_ADDRESS_A 0x28
#define IMU_ADDRESS_B 0x29
Adafruit_BNO055 bno1 = Adafruit_BNO055(55, IMU_ADDRESS_A, &Wire);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, IMU_ADDRESS_B, &Wire);
#define BNO055_SAMPLERATE_DELAY_MS (100)
bool calibrated;
int altitude_offset;

// LoRa vars ---------------------------------------
const String lora_band = "865000000"; //enter band as per your country
const String lora_networkid = "5";    //enter Lora Network ID
const String lora_address = "2";      //enter Lora address
const String lora_RX_address = "1";   //enter Lora RX address (for sending)

String incoming_string;
#define LORA_STOP_CHAR 'S'
#define LORA_TX 14
#define LORA_RX 15
EspSoftwareSerial::UART LoRaSerialPort;

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
void switchAntennaGivenGravity(sensors_event_t& gravity_event);
void switchAntenna(int num);

// Battery Voltage
float getBatteryVoltage(void);
