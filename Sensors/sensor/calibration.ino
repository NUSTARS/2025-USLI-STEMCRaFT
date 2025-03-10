#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>


void displaySensorDetails(Adafruit_BNO055& b)
{
    sensor_t sensor;
    b.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
    Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
    Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

void displaySensorStatus(Adafruit_BNO055& b)
{
    /* Get the system status values (mostly for debugging purposes) */
    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    b.getSystemStatus(&system_status, &self_test_results, &system_error);

    /* Display the results in the Serial Monitor */
    Serial.println("");
    Serial.print("System Status: 0x");
    Serial.println(system_status, HEX);
    Serial.print("Self Test:     0x");
    Serial.println(self_test_results, HEX);
    Serial.print("System Error:  0x");
    Serial.println(system_error, HEX);
    Serial.println("");
    delay(500);
}

void displayCalStatus(Adafruit_BNO055& b)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    b.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    // if (!system)
    // {
    //     Serial.print("! ");
    // }

    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
}


void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}

void bunnyPrintOrientation(double x, double y, double z) {
  Serial.print(F("Orientation: "));
  Serial.print((float)x);
  Serial.print(F(" "));
  Serial.print((float)y);
  Serial.print(F(" "));
  Serial.print((float)z);
  Serial.println(F(""));
}

void bunnyPrintCalibration(Adafruit_BNO055& b) {
  uint8_t sys, gyro, accel, mag = 0;
  b.getCalibration(&sys, &gyro, &accel, &mag);
  
  Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(" "));
  Serial.print(gyro, DEC);
  Serial.print(F(" "));
  Serial.print(accel, DEC);
  Serial.print(F(" "));
  Serial.print(mag, DEC);
}


bool cal_setup(Adafruit_BNO055& b, int address)
{
    bool zero = true;
    double degToRad = 57.295779513;
    
    Serial.println("Testing Calibration");

    int eeAddress = address;
    long bnoID;
    bool foundCalib = false;


    EEPROM.get(eeAddress, bnoID);

    Serial.print("Stored bnoID: "); Serial.println(bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
    */
    b.getSensor(&sensor);

    Serial.print("Actual bnoID: "); Serial.println(sensor.sensor_id);

    if (bnoID != sensor.sensor_id)
    {
        Serial.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

        displaySensorOffsets(calibrationData);

        Serial.println("\n\nRestoring Calibration data to the BNO055...");
        b.setSensorOffsets(calibrationData);

        Serial.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }

  //   delay(1000);

    /* Display some basic information on this sensor */
    displaySensorDetails(b);

    /* Optional: Display current status */
    displaySensorStatus(b);

  //  //Crystal must be configured AFTER loading calibration data into BNO055.
    b.setExtCrystalUse(true);

    sensors_event_t event;
    b.getEvent(&event);

    // RECALIB -> VARIABLE IN PAYLOAD
    // if (foundCalib && RECALIB == 0)
    if (foundCalib)
    {
        Serial.println("Move sensor slightly to calibrate magnetometers");
        while (!b.isFullyCalibrated())
        {
            b.getEvent(&event);
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
    }
    else
    {
        Serial.println("Please Calibrate Sensor: ");
        long long time = millis();
        while (!b.isFullyCalibrated())
        {
            if((millis()-time)/1000 >= 60) {//after 2 min fails
              return false;
            }
            b.getEvent(&event);

            imu::Vector<3> euler = b.getQuat().toEuler();
            
            double x = euler.y() * degToRad;
            double y = euler.z() * degToRad;
            double z = euler.x() * degToRad;
            
            Serial.print("X: ");
            Serial.print(x, 4);
            Serial.print(" Y: ");
            Serial.print(y, 4);
            Serial.print(" Z: ");
            Serial.print(z, 4);
            Serial.print("\t\t");

            /* Optional: Display calibration status */
            displayCalStatus(b);

            /* New line for the next sample */
            Serial.println("");

            /* Wait the specified delay before requesting new data */
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
    }

    Serial.println("\nFully calibrated!");
    Serial.println("--------------------------------");
    Serial.println("Calibration Results: ");
    adafruit_bno055_offsets_t newCalib;
    b.getSensorOffsets(newCalib);
    displaySensorOffsets(newCalib);

    Serial.println("\n\nStoring calibration data to EEPROM...");

    eeAddress = address;
    b.getSensor(&sensor);
    bnoID = sensor.sensor_id;

    EEPROM.put(eeAddress, bnoID);

    eeAddress += sizeof(long);

    Serial.print("Size of long: ");
    Serial.println(sizeof(long));
    
    EEPROM.put(eeAddress, newCalib);

    Serial.print("Size of new Calib: ");
    Serial.println(sizeof(newCalib));

    Serial.println("Data stored to EEPROM.");
    
    if(zero) {
      Serial.println("Zeroing... Please do not move the device");
      delay(1000);
    }
    
    b.setMode(OPERATION_MODE_GYRONLY);
    delay(500);
    return true;
}