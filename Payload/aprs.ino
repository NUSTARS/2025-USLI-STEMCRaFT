#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <stdio.h>
#include <avr/dtostrf.h>
#include <MemoryFree.h>;
#include <ZeroAPRS.h>                       //https://github.com/hakkican/ZeroAPRS
#include <SparkFun_Ublox_Arduino_Library.h> //https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

#define BattPin       A5
#define GpsPwr        7
#define PwDwPin       A3
#define PowerHL       A4
#define PttPin        3

//macros
#define GpsON       digitalWrite(GpsPwr, LOW)
#define GpsOFF      digitalWrite(GpsPwr, HIGH)
#define PttON       digitalWrite(PttPin, HIGH)
#define PttOFF      digitalWrite(PttPin, LOW)
#define RadioON     digitalWrite(PwDwPin, HIGH)
#define RadioOFF    digitalWrite(PwDwPin, LOW)
#define RfHiPwr     digitalWrite(PowerHL, HIGH)
#define RfLowPwr    digitalWrite(PowerHL, LOW)

//#define DEVMODE // Development mode. Uncomment to enable for debugging.

//******************************  APRS CONFIG **********************************
char    CallSign[7]="KK7YNC"; //DO NOT FORGET TO CHANGE YOUR CALLSIGN
int8_t  CallNumber=11;//SSID http://www.aprs.org/aprs11/SSIDs.txt
char    Symbol='>'; // 'O' for balloon, '>' for car, for more info : http://www.aprs.org/symbols/symbols-new.txt
bool    alternateSymbolTable = false ; //false = '/' , true = '\'

char Frequency[9]="144.3900"; //default frequency. 144.3900 for US, 144.8000 for Europe

char    comment[50] = "LightAPRS 2.0"; // Max 50 char but shorter is better.
// bool    FirstTime = true;
// char    Time[13] = "";
char    DataArray[63];

//*****************************************************************************

uint16_t  BeaconWait=50;  //seconds sleep for next beacon (HF or VHF). This is optimized value, do not change this if possible.
uint16_t  Interval=2;    //seconds between transmission attempts 
float     BattMin=3.3;    // min Volts to wake up.
float     DraHighVolt=5.0;    // min Volts for radio module (DRA818V) to transmit (TX) 1 Watt, below this transmit 0.5 Watt.

//******************************  APRS SETTINGS *********************************

//do not change WIDE path settings below if you don't know what you are doing :) 
uint8_t   Wide1=1; // 1 for WIDE1-1 path
uint8_t   Wide2=1; // 1 for WIDE2-1 path

/**
Airborne stations above a few thousand feet should ideally use NO path at all, or at the maximum just WIDE2-1 alone.  
Due to their extended transmit range due to elevation, multiple digipeater hops are not required by airborne stations.  
Multi-hop paths just add needless congestion on the shared APRS channel in areas hundreds of miles away from the aircraft's own location.  
NEVER use WIDE1-1 in an airborne path, since this can potentially trigger hundreds of home stations simultaneously over a radius of 150-200 miles. 
 */
uint8_t pathSize=2; // 2 for WIDE1-N,WIDE2-N ; 1 for WIDE2-N
boolean autoPathSizeHighAlt = true; //force path to WIDE2-N only for high altitude (airborne) beaconing (over 1.000 meters (3.280 feet)) 
boolean radioSetup = false; //do not change this, temp value
static char telemetry_buff[100];// telemetry buffer
uint16_t TxCount = 1; //increased +1 after every APRS transmission

//******************************  GPS SETTINGS   *********************************
int16_t   GpsResetTime=1800; // timeout for reset if GPS is not fixed
boolean ublox_high_alt_mode_enabled = false; //do not change this
int16_t GpsInvalidTime=0; //do not change this
boolean gpsSetup=false; //do not change this.

//********************************************************************************

SFE_UBLOX_GPS myGPS;
// Adafruit_BMP085 bmp;


void receiveData(int NumBytes) {
  // if (FirstTime) {
  //   strcat(Time, (const char*) myGPS.getHour());
  //   strcat(Time, ":");
  //   strcat(Time, (const char*) myGPS.getMinute());
  //   strcat(Time, ":");
  //   strcat(Time, (const char*) myGPS.getSecond());
  //   FirstTime = false;
  // }

  SerialUSB.println(F("Inside Handler"));

  int i = 0;
  char var;

  while (Wire.available()) {
    var = Wire.read();

    if (var == 0) {
      break;
    }

    DataArray[i] = var;
    i++;
  }

  strcat(DataArray, Time);

  if (DataArray[0] != '\0') {
    SerialUSB.println(DataArray);
    sendStatus();
  }
  DataArray[0] = '\0';
}

void setup() {
  // While the energy rises slowly with the solar panel, 
  // using the analog reference low solves the analog measurement errors.
  analogReference(AR_INTERNAL1V65);
  pinMode(PttPin, OUTPUT);
  pinMode(GpsPwr, OUTPUT);
  pinMode(BattPin, INPUT);
  pinMode(PwDwPin, OUTPUT);
  pinMode(PowerHL, OUTPUT);
  
  GpsOFF;
  PttOFF;
  RadioOFF; 
  RfLowPwr;

  SerialUSB.begin(9600);
  // Wait up to 5 seconds for serial to be opened, to allow catching
  // startup messages on native USB boards (that do not reset when
  // serial is opened).
  //Watchdog.reset();  
  unsigned long start = millis();
  while (millis() - start < 5000 && !SerialUSB){;}
  //Watchdog.reset(); 

  SerialUSB.println(F("Starting"));
  Serial1.begin(9600);// for DorjiDRA818V

  APRS_init();
  APRS_setCallsign(CallSign, CallNumber);
  APRS_setDestination("APLIGA", 0);
  APRS_setPath1("WIDE1", Wide1);
  APRS_setPath2("WIDE2", Wide2);
  APRS_setPathSize(2);
  APRS_useAlternateSymbolTable(alternateSymbolTable);
  APRS_setSymbol(Symbol);
  APRS_setPathSize(pathSize);
  APRS_setGain(2);

  configDra818(Frequency);

  Wire.begin(0x08);
  Wire.onReceive(receiveData);

  SerialUSB.println(F(""));
  SerialUSB.print(F("APRS (VHF) CallSign: "));
  SerialUSB.print(CallSign);
  SerialUSB.print(F("-"));
  SerialUSB.println(CallNumber);

}

void loop() {
  // sendStatus();
  // SerialUSB.println(F("Loop"));
  
  // if (readBatt() > BattMin) {

  //   SerialUSB.println(F("Inside first If"));
    
  //   if (DataArray != "") {	
  //     SerialUSB.println(DataArray);	   	  
  //     strcpy(DataArray, "");

  //     while (readBatt() < BattMin) {
  //       sleepSeconds(Interval); 
  //     }
   
  //   } 
      
  //     // if(!gpsSetup) 
  //     // {
  //     //   SerialUSB.println(F("Inside GPS"));
  //     //   gpsStart();
  //     // }
      
  //     // Models for GPS: DYN_MODEL_PORTABLE, DYN_MODEL_STATIONARY, DYN_MODEL_PEDESTRIAN, DYN_MODEL_AUTOMOTIVE, DYN_MODEL_SEA, 
  //     // DYN_MODEL_AIRBORNE1g, DYN_MODEL_AIRBORNE2g, DYN_MODEL_AIRBORNE4g, DYN_MODEL_WRIST, DYN_MODEL_BIKE
  //     // DYN_MODEL_PORTABLE is suitable for most situations except airborne vehicles.      
  //     if(!ublox_high_alt_mode_enabled){setupUBloxDynamicModel(DYN_MODEL_AIRBORNE4g);}
      
  //     if (myGPS.getPVT()) {
  //       gpsDebug();
  //       if ( (myGPS.getFixType() != 0) && (myGPS.getSIV() > 3) ) {
  //         GpsInvalidTime=0;
  //         updatePosition();
  //         updateTelemetry();

  //         if(autoPathSizeHighAlt && ((myGPS.getAltitude() * 3.2808399)  / 1000.f) > 3000){
  //           //force to use high altitude settings (WIDE2-n)
  //           APRS_setPathSize(1);
  //           } else {
  //           //use default settings  
  //           APRS_setPathSize(pathSize);
  //         }

  //         sendLocation();
  //         freeMem();
  //         SerialUSB.flush();
  //         sleepSeconds(BeaconWait);       
          
  //       }else{
  //         GpsInvalidTime++;
  //         if(GpsInvalidTime > GpsResetTime){
  //           GpsOFF;
  //           ublox_high_alt_mode_enabled = false; //gps sleep mode resets high altitude mode.
            
  //           GpsON;
  //           GpsInvalidTime=0;     
  //         }
  //       }
  //     } else {
  //       #if defined(DEVMODE)
  //       SerialUSB.println(F("Not enough sattelites"));
  //       #endif
  //     }

    
  // } else {
  //   sleepSeconds(Interval);
  // }
}

void gpsStart(){  
  bool gpsBegin=false;  
  while(!gpsBegin){
    GpsON;
    delay(1000);
    gpsBegin=myGPS.begin();
    if(gpsBegin)break;
    #if defined(DEVMODE)  
    SerialUSB.println(F("Ublox GPS not detected at default I2C address. Will try again"));
    #endif 
    delay(2000);
  }
   // do not overload the buffer system from the GPS, disable UART output
  myGPS.setUART1Output(0); //Disable the UART1 port output 
  myGPS.setUART2Output(0); //Disable Set the UART2 port output
  myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGPS.saveConfiguration(); //Save the current settings to flash and BBR  
  gpsSetup=true;
}

void sleepSeconds(int sec) {
  PttOFF;
  RadioOFF;

  SerialUSB.flush();
  for (int i = 0; i < sec; i++) {
    delay(1000);   
  }

}

byte configDra818(char *freq)
{
  RadioON;
  char ack[3];
  int n;
  delay(2000);
  char cmd[50];//"AT+DMOSETGROUP=0,144.8000,144.8000,0000,4,0000"
  sprintf(cmd, "AT+DMOSETGROUP=0,%s,%s,0000,4,0000", freq, freq);
  Serial1.println(cmd);
  SerialUSB.println("RF Config");
  ack[2] = 0;
  while (ack[2] != 0xa)
  {
    if (Serial1.available() > 0) {
      ack[0] = ack[1];
      ack[1] = ack[2];
      ack[2] = Serial1.read();
    }
  }
  delay(2000);
  RadioOFF;

  if (ack[0] == 0x30) {
      SerialUSB.print(F("Frequency updated: "));
      SerialUSB.print(freq);
      SerialUSB.println(F("MHz"));
    } else {
      SerialUSB.println(F("Frequency update error!!!"));    
    }
  return (ack[0] == 0x30) ? 1 : 0;
}

void updatePosition() {
  // Convert and set latitude NMEA string Degree Minute Hundreths of minutes ddmm.hh[S,N].
  char latStr[10];
  int temp = 0;
  double d_lat = myGPS.getLatitude() / 10000000.f;
  double dm_lat = 0.0;

  if (d_lat < 0.0) {
    temp = -(int)d_lat;
    dm_lat = temp * 100.0 - (d_lat + temp) * 60.0;
  } else {
    temp = (int)d_lat;
    dm_lat = temp * 100 + (d_lat - temp) * 60.0;
  }

  dtostrf(dm_lat, 7, 2, latStr);

  if (dm_lat < 1000) {
    latStr[0] = '0';
  }

  if (d_lat >= 0.0) {
    latStr[7] = 'N';
  } else {
    latStr[7] = 'S';
  }

  APRS_setLat(latStr);

  // Convert and set longitude NMEA string Degree Minute Hundreths of minutes ddmm.hh[E,W].
  char lonStr[10];
  double d_lon = myGPS.getLongitude() / 10000000.f;
  double dm_lon = 0.0;

  if (d_lon < 0.0) {
    temp = -(int)d_lon;
    dm_lon = temp * 100.0 - (d_lon + temp) * 60.0;
  } else {
    temp = (int)d_lon;
    dm_lon = temp * 100 + (d_lon - temp) * 60.0;
  }

  dtostrf(dm_lon, 8, 2, lonStr);

  if (dm_lon < 10000) {
    lonStr[0] = '0';
  }
  if (dm_lon < 1000) {
    lonStr[1] = '0';
  }

  if (d_lon >= 0.0) {
    lonStr[8] = 'E';
  } else {
    lonStr[8] = 'W';
  }

  APRS_setLon(lonStr);
  APRS_setTimeStamp(myGPS.getHour(), myGPS.getMinute(), myGPS.getSecond());
}

void updateTelemetry() {
  sprintf(telemetry_buff, "%03d", (int)(myGPS.getHeading() / 100000));
  telemetry_buff[3] += '/';
  sprintf(telemetry_buff + 4, "%03d", (int)(myGPS.getGroundSpeed() * 0.00194384f));
  telemetry_buff[7] = '/';
  telemetry_buff[8] = 'A';
  telemetry_buff[9] = '=';
  //fixing negative altitude values causing display bug on aprs.fi
  float tempAltitude = (myGPS.getAltitude() * 3.2808399)  / 1000.f;

  if (tempAltitude > 0) {
    //for positive values
    sprintf(telemetry_buff + 10, "%06lu", (long)tempAltitude);
  } else {
    //for negative values
    sprintf(telemetry_buff + 10, "%06d", (long)tempAltitude);
  }
  telemetry_buff[16] = ' ';
  sprintf(telemetry_buff + 17, "%03d", TxCount);
  telemetry_buff[20] = 'T';
  telemetry_buff[21] = 'x';
  telemetry_buff[22] = 'C';
  // telemetry_buff[23] = ' '; float tempC = bmp.readTemperature();
  // dtostrf(tempC, 6, 2, telemetry_buff + 24);
  telemetry_buff[30] = 'C';
  // telemetry_buff[31] = ' '; float pressure = bmp.readPressure() / 100.0; //Pa to hPa
  // dtostrf(pressure, 7, 2, telemetry_buff + 32);
  telemetry_buff[39] = 'h';
  telemetry_buff[40] = 'P';
  telemetry_buff[41] = 'a';
  telemetry_buff[42] = ' ';
  dtostrf(readBatt(), 5, 2, telemetry_buff + 43);
  telemetry_buff[48] = 'V';
  telemetry_buff[49] = ' ';
  sprintf(telemetry_buff + 50, "%02d", (int)myGPS.getSIV()); //Returns number of sats used in fix
  telemetry_buff[52] = 'S';
  telemetry_buff[53] = ' ';
  sprintf(telemetry_buff + 54, "%s", comment);

  #if defined(DEVMODE)
  SerialUSB.println(telemetry_buff);
  #endif
}

void sendLocation() {
  SerialUSB.println(F("Location sending with comment..."));
  if (readBatt() > DraHighVolt) RfHiPwr; //DRA Power 1 Watt
  else RfLowPwr; //DRA Power 0.5 Watt
  RadioON;
  delay(2000);
  PttON;
  delay(1000);  
  APRS_sendLoc(telemetry_buff);
  delay(10);
  PttOFF;
  RadioOFF;
  SerialUSB.print(F("Location sent with comment - "));
  SerialUSB.println(TxCount);  
  TxCount++;
}

void sendStatus() {

  SerialUSB.println(F("Status sending..."));
  if (readBatt() > DraHighVolt) RfHiPwr; //DRA Power 1 Watt
  else RfLowPwr; //DRA Power 0.5 Watt
  RadioON;
  delay(2000);
  PttON;
  delay(1000);
  APRS_sendStatus(DataArray);
  delay(10);
  PttOFF;
  RadioOFF;
  delay(1000);
  SerialUSB.print(F("Status sent - "));
  SerialUSB.println(TxCount);
  TxCount++;
}

void gpsDebug() { 
#if defined(DEVMODE)
    byte fixType = myGPS.getFixType();
    SerialUSB.print(F("FixType: "));
    SerialUSB.print(fixType);    

    int SIV = myGPS.getSIV();
    SerialUSB.print(F(" Sats: "));
    SerialUSB.print(SIV);

    float flat = myGPS.getLatitude() / 10000000.f;    
    SerialUSB.print(F(" Lat: "));
    SerialUSB.print(flat);    

    float flong = myGPS.getLongitude() / 10000000.f;    
    SerialUSB.print(F(" Long: "));
    SerialUSB.print(flong);        

    float altitude = myGPS.getAltitude() / 1000;
    SerialUSB.print(F(" Alt: "));
    SerialUSB.print(altitude);
    SerialUSB.print(F(" (m)"));

    float speed = myGPS.getGroundSpeed();
    SerialUSB.print(F(" Speed: "));
    SerialUSB.print(speed * 0.00194384f);
    SerialUSB.print(F(" (kn/h)"));    
        
    SerialUSB.print(" Time: ");    
    SerialUSB.print(myGPS.getYear());
    SerialUSB.print("-");
    SerialUSB.print(myGPS.getMonth());
    SerialUSB.print("-");
    SerialUSB.print(myGPS.getDay());
    SerialUSB.print(" ");
    SerialUSB.print(myGPS.getHour());
    SerialUSB.print(":");
    SerialUSB.print(myGPS.getMinute());
    SerialUSB.print(":");
    SerialUSB.print(myGPS.getSecond());
    
    // SerialUSB.print(" Temp: ");
    // SerialUSB.print(bmp.readTemperature());
    // SerialUSB.print(" C");
    
    // SerialUSB.print(" Press: ");    
    // SerialUSB.print(bmp.readPressure() / 100.0);
    // SerialUSB.print(" hPa");
    SerialUSB.println();  

#endif
}

void setupUBloxDynamicModel(dynModel newDynamicModel) {
  // If we are going to change the dynamic platform model, let's do it here.
  // Possible values are:
  //DYN_MODEL_PORTABLE //Applications with low acceleration, e.g. portable devices. Suitable for most situations.
  //DYN_MODEL_STATIONARY //Used in timing applications (antenna must be stationary) or other stationary applications. Velocity restricted to 0 m/s. Zero dynamics assumed.
  //DYN_MODEL_PEDESTRIAN   //Applications with low acceleration and speed, e.g. how a pedestrian would move. Low acceleration assumed.
  //DYN_MODEL_AUTOMOTIVE   //Used for applications with equivalent dynamics to those of a passenger car. Low vertical acceleration assumed
  //DYN_MODEL_SEA        //Recommended for applications at sea, with zero vertical velocity. Zero vertical velocity assumed. Sea level assumed.
  //DYN_MODEL_AIRBORNE1g   //Airborne <1g acceleration. Used for applications with a higher dynamic range and greater vertical acceleration than a passenger car. No 2D position fixes supported.
  //DYN_MODEL_AIRBORNE2g   //Airborne <2g acceleration. Recommended for typical airborne environments. No 2D position fixes supported.
  //DYN_MODEL_AIRBORNE4g   //Airborne <4g acceleration. Only recommended for extremely dynamic environments. No 2D position fixes supported.
  //DYN_MODEL_WRIST      // Not supported in protocol versions less than 18. Only recommended for wrist worn applications. Receiver will filter out arm motion.
  //DYN_MODEL_BIKE       // Supported in protocol versions 19.2

    if (myGPS.setDynamicModel(newDynamicModel) == false) // Set the dynamic model to DYN_MODEL_AIRBORNE4g
    {
      #if defined(DEVMODE)
        SerialUSB.println(F("***!!! Warning: setDynamicModel failed !!!***"));
      #endif 
    }
    else
    {
      ublox_high_alt_mode_enabled = true;
      #if defined(DEVMODE)
        SerialUSB.print(F("Dynamic platform model changed successfully! : "));
        SerialUSB.println(myGPS.getDynamicModel());
      #endif  
    }
  
  } 

float readBatt() {

  float R1 = 560000.0; // 560K
  float R2 = 100000.0; // 100K
  float value = 0.0f;

  do {    
    value =analogRead(BattPin);
    value +=analogRead(BattPin);
    value +=analogRead(BattPin);
    value = value / 3.0f;
    value = (value * 1.65) / 1024.0f;
    value = value / (R2/(R1+R2));
  } while (value > 20.0);
  return value ;

}

void freeMem() {
#if defined(DEVMODE)
  SerialUSB.print(F("Free RAM: ")); SerialUSB.print(freeMemory(), DEC); SerialUSB.println(F(" byte"));
#endif

}
