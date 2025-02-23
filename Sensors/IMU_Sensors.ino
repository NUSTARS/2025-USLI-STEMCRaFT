#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// delay between each sample
uint16_t BNO055_SAMPLERATE_DELAY_MS = 1000;

// number of datapoints: orientation, linear, gravity
const int NUMBER_DATAPOINTS = 3;

// sensor connection status
bool firstSensorFound;
bool secondSensorFound;

// Use pointers so we can reinitialize when needed
Adafruit_BNO055 *bno = nullptr;
Adafruit_BNO055 *bno2 = nullptr;

//
// calculate averages
//
float average(float input1, float input2) 
{
  return (input1 + input2) / 2;
}

//
// check if sensor data is all equal to 0, which indicates sensor disconnected
//
bool checkEmpty(sensors_event_t events[]) 
{
  for (int i = 0; i < NUMBER_DATAPOINTS; i++) 
  {
    double x = 0, y = 0, z = 0;

    if (events[i].type == SENSOR_TYPE_ORIENTATION) 
    {
      x = events[i].orientation.x;
      y = events[i].orientation.y;
      z = events[i].orientation.z;
    } 
    else if (events[i].type == SENSOR_TYPE_LINEAR_ACCELERATION) 
    {
      x = events[i].acceleration.x;
      y = events[i].acceleration.y;
      z = events[i].acceleration.z;
    } 
    else if (events[i].type == SENSOR_TYPE_GRAVITY) 
    {
      x = events[i].acceleration.x;
      y = events[i].acceleration.y;
      z = events[i].acceleration.z;
    }

    if (x != 0 || y != 0 || z != 0) 
    {
      return false;
    }
  }

  return true;
}

//
// Initialize sensor objects and update connection flags
//
void initializeSensors() 
{
  // Sensor 1 initialization
  if (bno != nullptr) 
  {
    delete bno;
  }

  bno = new Adafruit_BNO055(1, 0x28, &Wire);
  firstSensorFound = bno->begin();
  
  if (firstSensorFound) 
  {
    bno->setExtCrystalUse(true);
    Serial.println("Sensor 1 initialized.");
  } 
  else 
  {
    Serial.println("Sensor 1 initialization failed.");
  }

  // Sensor 2 initialization
  if (bno2 != nullptr) 
  {
    delete bno2;
  }

  bno2 = new Adafruit_BNO055(2, 0x29, &Wire);
  secondSensorFound = bno2->begin();

  if (secondSensorFound) 
  {
    bno2->setExtCrystalUse(true);
    Serial.println("Sensor 2 initialized.");
  } 
  else 
  {
    Serial.println("Sensor 2 initialization failed.");
  }
}


void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) delay(10);
  Wire.begin();

  // Initialize sensors on startup
  initializeSensors();
  delay(1000);
}


void loop(void) 
{
  if (!firstSensorFound) 
  {
    Serial.println("Attempting to reconnect Sensor 1...");
    if (bno != nullptr) 
    {
      delete bno;
    }

    bno = new Adafruit_BNO055(1, 0x28, &Wire);
    if (bno->begin()) 
    {
      firstSensorFound = true;
      bno->setExtCrystalUse(true);
      Serial.println("Sensor 1 reconnected");
    }
  }

  // For Sensor 2:
  if (!secondSensorFound) 
  {
    Serial.println("Attempting to reconnect Sensor 2...");

    if (bno2 != nullptr) 
    {
      delete bno2;
    }

    bno2 = new Adafruit_BNO055(2, 0x29, &Wire);
    
    if (bno2->begin()) 
    {
      secondSensorFound = true;
      bno2->setExtCrystalUse(true);
      Serial.println("Sensor 2 reconnected successfully!");
    }
  }

  // Set up data arrays for sensor events
  sensors_event_t data[NUMBER_DATAPOINTS];
  sensors_event_t data2[NUMBER_DATAPOINTS];

  // Get sensor events only if sensor is connected; otherwise, zero out the events
  bno->getEvent(&data[0], Adafruit_BNO055::VECTOR_EULER);
  bno->getEvent(&data[1], Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno->getEvent(&data[2], Adafruit_BNO055::VECTOR_GRAVITY);

  bno2->getEvent(&data2[0], Adafruit_BNO055::VECTOR_EULER);
  bno2->getEvent(&data2[1], Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno2->getEvent(&data2[2], Adafruit_BNO055::VECTOR_GRAVITY);

  // Update sensor connection status based on current readings
  firstSensorFound = !checkEmpty(data);
  secondSensorFound = !checkEmpty(data2);

  // Report sensor status and print events
  if (!firstSensorFound && !secondSensorFound) 
  {
    Serial.println("Both Sensors Disconnected");
  } 
  else if (!firstSensorFound) 
  {
    Serial.println("Sensor 1 disconnected; only using Sensor 2");
    printEvents(data);
    printEvents(data2);
  } 
  else if (!secondSensorFound) 
  {
    Serial.println("Sensor 2 disconnected; only using Sensor 1");
    printEvents(data);
    printEvents(data2);
  } 
  else 
  {
    Serial.println("Both Sensors Connected");
    printEvents(data);
    printEvents(data2);
  }

  Serial.println("--");
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

//
// output each data point from individual sensors
//
void printEvents(sensors_event_t events[]) 
{
  for (int i = 0; i < NUMBER_DATAPOINTS; i++) 
  {
    double x = -1000000, y = -1000000, z = -1000000; // obviously incorrect if not updated
    
    if (events[i].type == SENSOR_TYPE_ORIENTATION) 
    {
      Serial.print("Orient: ");
      
      x = events[i].orientation.x;
      y = events[i].orientation.y;
      z = events[i].orientation.z;
    } 
    else if (events[i].type == SENSOR_TYPE_LINEAR_ACCELERATION) 
    {
      Serial.print("Linear: ");

      x = events[i].acceleration.x;
      y = events[i].acceleration.y;
      z = events[i].acceleration.z;
    } 
    else if (events[i].type == SENSOR_TYPE_GRAVITY) 
    {
      Serial.print("Gravity: ");

      x = events[i].acceleration.x;
      y = events[i].acceleration.y;
      z = events[i].acceleration.z;
    } 
    else 
    {
      Serial.print("Unk: ");
    }

    Serial.print("\tx= ");
    Serial.print(x);
    Serial.print(" |\ty= ");
    Serial.print(y);
    Serial.print(" |\tz= ");
    Serial.println(z);
  }
}
