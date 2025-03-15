//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>

// formatting of output vectors
#include <ArduinoEigen.h>

#include <EEPROM.h>



// delay between each samples
//uint16_t BNO055_SAMPLERATE_DELAY_MS = 1000;

// number of datapoints: orientation, linear, gravity
const int NUMBER_DATAPOINTS = 3;

// check if the sensors are still connected
bool firstSensorFound;
bool secondSensorFound;

// initialize isCalibrated?
bool isFirstBNOCalibrated;
bool isSecondBNOCalibrated;

// set up each data array
sensors_event_t data[NUMBER_DATAPOINTS];
sensors_event_t data2[NUMBER_DATAPOINTS];

// initialize each Adafruit
/*
Adafruit_BNO055 bno = Adafruit_BNO055(1, 0x28, &Wire);
Adafruit_BNO055 bno2 = Adafruit_BNO055(2, 0x28, &Wire);
*/

//
// calculate averages
//
float average(float input1, float input2);

//
// check if sensor data is all equal, which indicates sensor disconnected
//
bool checkEmpty(sensors_event_t events[]);

//
// output each data point from individual sensors
//
void printEvents(sensors_event_t events[]); 

// 
// getter for linear acceleration: returns vector of 3 floats
//
Eigen::Vector3f getLinearAcceleration();

// 
// getter for gravity: returns vector of 3 floats
//
Eigen::Vector3f getGravity();

// 
// getter for orientation: returns vector of 3 floats
//
Eigen::Vector3f getOrientation();

//
// output average data from the two sensors
//
void printAverageEvents(sensors_event_t events[], sensors_event_t events2[]); 

//
// setup function for bnos
//
void setupBNOs();



void imuDataHelper(void)
{
  EEPROM.begin(EEPROM_SIZE*2);

  // call calibration function for the first bno
  if (!isFirstBNOCalibrated)
  {
    isFirstBNOCalibrated = cal_setup(bno1, 0);
  }

  // call calibration for the second bno
  if (!isSecondBNOCalibrated)
  {
    isSecondBNOCalibrated = cal_setup(bno2, 26);
  }

  bno1.setMode(OPERATION_MODE_GYRONLY);
  bno2.setMode(OPERATION_MODE_GYRONLY);

  EEPROM.commit();
  EEPROM.end();

  // // call calibration for second sensor of bno
  // calibration_setup(bno2, data2);

  // fill values of data; order of data: orientation, linearAccel, gravity
  bno1.getEvent(&data[0], Adafruit_BNO055::VECTOR_EULER);
  bno1.getEvent(&data[1], Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno1.getEvent(&data[2], Adafruit_BNO055::VECTOR_GRAVITY);

  // fill values of data2
  bno2.getEvent(&data2[0], Adafruit_BNO055::VECTOR_EULER);
  bno2.getEvent(&data2[1], Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno2.getEvent(&data2[2], Adafruit_BNO055::VECTOR_GRAVITY);

  // if the sensors all have 0, must be disconnected
  firstSensorFound = !checkEmpty(data);
  secondSensorFound = !checkEmpty(data2);
  
  
  if (!firstSensorFound && !secondSensorFound) 
  {
    Serial.println("Both Sensors Disconnected");
    while(1);
  }
  else if (!firstSensorFound) 
  {
    Serial.println("Sensor 1 disconnected; only using sensor two");
    printEvents(data);
    printEvents(data2);
  }
  else if (!secondSensorFound) 
  {
    Serial.println("Sensor 2 disconnected; only using sensor one");
    printEvents(data);
    printEvents(data2);
  } 
  else 
  {
    Serial.println("Both Sensors Connected");

    // print out the averages of each data
    printAverageEvents(data, data2); 

    // // test if each sensor works
    // printEvents(data);
    // printEvents(data2);
  }

  Serial.println("--");
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}

// 
// getter for linear acceleration: returns vector of 3 floats
//
Eigen::Vector3f getLinearAcceleration() 
{
  float x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

  if (!firstSensorFound && !secondSensorFound) // no sensors connected
  {
    Serial.print("No sensors detected");
  }
  else if (!firstSensorFound) // only second sensor connected
  {
    // use data2 variable
    x = data2[1].acceleration.x;
    y = data2[1].acceleration.y;
    z = data2[1].acceleration.z;
  }
  else if (!secondSensorFound) // only first sensor connected
  {
    // use data variable
    x = data[1].acceleration.x;
    y = data[1].acceleration.y;
    z = data[1].acceleration.z;
  } 
  else // both sensors connected
  {
    x = (data[1].acceleration.x + data2[1].acceleration.x)/2;
    y = (data[1].acceleration.y + data2[1].acceleration.y)/2;
    z = (data[1].acceleration.z + data2[1].acceleration.z)/2;
  }

  Eigen::Vector3f v;

  v << x, y, z;

  return v;
}

//
// setup function for bnos
//
void setupBNOs() 
{
  Serial.begin(115200);

  while (!Serial) delay(10); 

  // start each sensors; if false, not found the sensor
  firstSensorFound = bno1.begin();
  secondSensorFound = bno2.begin();

  isFirstBNOCalibrated = false;
  isSecondBNOCalibrated = false;
}

// 
// getter for gravity: returns vector of 3 floats
//
Eigen::Vector3f getGravity() 
{
  float x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

  if (!firstSensorFound && !secondSensorFound) // no sensors connected
  {
    Serial.print("No sensors detected");
  }
  else if (!firstSensorFound) // only second sensor connected
  {
    // use data2 variable
    x = data2[2].acceleration.x;
    y = data2[2].acceleration.y;
    z = data2[2].acceleration.z;
  }
  else if (!secondSensorFound) // only first sensor connected
  {
    // use data variable
    x = data[2].acceleration.x;
    y = data[2].acceleration.y;
    z = data[2].acceleration.z;
  } 
  else // both sensors connected
  {
    x = (data[2].acceleration.x + data2[2].acceleration.x)/2;
    y = (data[2].acceleration.y + data2[2].acceleration.y)/2;
    z = (data[2].acceleration.z + data2[2].acceleration.z)/2;
  }

  Eigen::Vector3f v;

  v << x, y, z;

  return v;
}

// 
// getter for orientation: returns vector of 3 floats
//
Eigen::Vector3f getOrientation() 
{
  float x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

  if (!firstSensorFound && !secondSensorFound) // no sensors connected
  {
    Serial.print("No sensors detected");
  }
  else if (!firstSensorFound) // only second sensor connected
  {
    // use data2 variable
    x = data2[0].orientation.x;
    y = data2[0].orientation.y;
    z = data2[0].orientation.z;
  }
  else if (!secondSensorFound) // only first sensor connected
  {
    // use data variable
    x = data[0].orientation.x;
    y = data[0].orientation.y;
    z = data[0].orientation.z;
  } 
  else // both sensors connected
  {
    x = (data[0].orientation.x + data2[0].orientation.x)/2;
    y = (data[0].orientation.y + data2[0].orientation.y)/2;
    z = (data[0].orientation.z + data2[0].orientation.z)/2;
  }

  Eigen::Vector3f v;

  v << x, y, z;

  return v;
}


//
// output each data point from individual sensors
//
void printEvents(sensors_event_t events[]) 
{
  for (int i = 0; i < NUMBER_DATAPOINTS; i++)
  {
    double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

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
      Serial.print("Unk:");
    }

    Serial.print("\tx= ");
    Serial.print(x);
    Serial.print(" |\ty= ");
    Serial.print(y);
    Serial.print(" |\tz= ");
    Serial.println(z);
  }
}

//
// output average data from the two sensors
//
void printAverageEvents(sensors_event_t events[], sensors_event_t events2[]) 
{
  for (int i = 0; i < NUMBER_DATAPOINTS; i++) 
  {
    double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem

    if (events[i].type == SENSOR_TYPE_ORIENTATION) 
    {
      Serial.print("Average Orient: ");

      x = average(events[i].orientation.x, events2[i].orientation.x);
      y = average(events[i].orientation.y, events2[i].orientation.y);
      z = average(events[i].orientation.z, events2[i].orientation.z);
    }
    else if (events[i].type == SENSOR_TYPE_LINEAR_ACCELERATION) 
    {
      Serial.print("Average Linear: ");

      x = average(events[i].acceleration.x, events2[i].acceleration.x);
      y = average(events[i].acceleration.y, events2[i].acceleration.y);
      z = average(events[i].acceleration.z, events2[i].acceleration.z);
    }
    else if (events[i].type == SENSOR_TYPE_GRAVITY) 
    {
      Serial.print("Average Gravity: ");

      x = average(events[i].acceleration.x, events2[i].acceleration.x);
      y = average(events[i].acceleration.y, events2[i].acceleration.y);
      z = average(events[i].acceleration.z, events2[i].acceleration.z);
    }
    else 
    {
      Serial.print("Unk:");
    }

    Serial.print("\tx= ");
    Serial.print(x);
    Serial.print(" |\ty= ");
    Serial.print(y);
    Serial.print(" |\tz= ");
    Serial.println(z);

  }
}

float average(float input1, float input2) 
{
  return (input1+input2)/2;
}

bool checkEmpty(sensors_event_t events[]) 
{
  for (int i = 0; i < NUMBER_DATAPOINTS; i++)
  {
    bool all_zeros = false;
    double x = 0, y = 0 , z = 0; //dumb values, easy to spot problem

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