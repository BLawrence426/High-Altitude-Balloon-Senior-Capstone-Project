//--------------------------------------------------------------------------------------------------------------
// These are all the libraries that need to be loaded in for this code to run
//--------------------------------------------------------------------------------------------------------------

#include <OneWire.h>            // This is for the temperature sensors
#include <DallasTemperature.h>  // This is for the temperature sensors
#include <SPI.h>                // This is to communicate with SPI devices (In this case: Pressure Sensor, Temperature Sensor, Radio Module)
#include <Adafruit_MPRLS.h>     // This is for the pressure sensor
#include <Wire.h>               // This is for the pressure sensor
#include <SD.h>                 // This is for saving backup data to the SD card
#include <Arduino_MKRGPS.h>     // This is for the GPS shield
#include <LoRa.h>               // This is for the MKR WAN 1310 board / Radio Module
#include <RTClib.h>             // This is for the Real Time Clock board

//--------------------------------------------------------------------------------------------------------------
// This is for the temperature sensors
//--------------------------------------------------------------------------------------------------------------

// This is to define what pins each temp. sensor data wire is connected to (Should be a digital pin)
#define ONE_WIRE_BUS 7  // All temperature sensors connected to pin# 1

// This is to define the precision of the temperature sensors
#define TEMPERATURE_PRECISION 12  // Sets the OneWire precision Dallas Sensor (12 bits = 0.0625°C)

// This is to pass OneWire references to Dallas Temperature library
OneWire oneWire(ONE_WIRE_BUS);        // Pull OneWire Reference
DallasTemperature sensors(&oneWire);  // Transfer OneWire reference to Dallas Temperature

// This is denoting which temperature sensor is interior/exterior.
// To get their addresses, run the "Multiple" example code in order to determine which address is what sensor

  // These addresses are for the MAIN Balloon
  //DeviceAddress InsideThermometer = { 0x28, 0x6F, 0x5E, 0x58, 0xD4, 0xE1, 0x3C, 0x21 };
  //DeviceAddress OutsideThermometer = { 0x28, 0x91, 0x82, 0x58, 0xD4, 0xE1, 0x3C, 0x3F };

  // These addresses are for the BACKUP Balloon
  DeviceAddress InsideThermometer = { 0x28, 0x03, 0x6E, 0x80, 0xE3, 0xE1, 0x3C, 0xA2 };
  DeviceAddress OutsideThermometer = { 0x28, 0x5B, 0x45, 0x80, 0xE3, 0xE1, 0x3C, 0x35 };

//--------------------------------------------------------------------------------------------------------------
// This is for the pressure sensor
//--------------------------------------------------------------------------------------------------------------

// This is to define the pins for Reset and EOC (End of Conversation). This isn't using them so we will define those to the Arduino reset pin
#define RESET_PIN -1  // Connected to Arduino reset pin (pin# -1). Change to anything else if you desire
#define EOC_PIN -1    // Connected to Arduino reset pin (pin# -1). Change to anything else if you desire
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

//--------------------------------------------------------------------------------------------------------------
// This is for the camera and SD card
//--------------------------------------------------------------------------------------------------------------

// Define the pins for the camera and SD card
#define SD_PIN 1               // This is the pin that the CS leg of thecamera is connected to
#define Trigger_PIN 0           // This is the pin that tells the camera to take a photo
int photoNumber = 0;            // This keeps track of the photo number

// This creates a File object to store the file
File BackUpData;
File PhotoData;

// Define timers to photos
const int Picture_Interval_MS = 15000;   // This is the timer for taking pictures
int Next_Picture_At = millis() + 5000;

//--------------------------------------------------------------------------------------------------------------
// This is for the LoRa Module on the Arduino MKR WAN 1310
//--------------------------------------------------------------------------------------------------------------

// Counter to keep track of every packet of information
int counter = 0;

//--------------------------------------------------------------------------------------------------------------
// This is for the RTC chip (Real Time Clock)
//--------------------------------------------------------------------------------------------------------------

// Initialzes the rtc chip
RTC_PCF8523 rtc;

void setup() {
  //----------------------------------------------------------------------------------------------------------------
  // This is all the setup code that gets ran only once (upon startup)
  //----------------------------------------------------------------------------------------------------------------
  Serial.begin(115200);
  while (!Serial) {
    ;  // This waits for the serial port to connect
  }

  //--------------------------------------------------------------------------------------------------------------
  // This is for setting up the trigger pin for the camera
  //--------------------------------------------------------------------------------------------------------------
  pinMode(Trigger_PIN, OUTPUT);
  digitalWrite(Trigger_PIN, LOW);

  //--------------------------------------------------------------------------------------------------------------
  // This is for the temperature sensors
  //--------------------------------------------------------------------------------------------------------------

  // Starts up library / sensors
  sensors.begin();  // This starts everything up
  Serial.println("Intializing temperature sensors...");
  delay(1000);
  if (sensors.getDeviceCount() == 0) {
    Serial.println("Failed to initialize temperature sensor");
  } else Serial.println("Temperature sensors ready!");
  delay(1000);       // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space

  //--------------------------------------------------------------------------------------------------------------
  // This is to set the precision of both temperature sensor
  //--------------------------------------------------------------------------------------------------------------

  sensors.setResolution(InsideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(OutsideThermometer, TEMPERATURE_PRECISION);

  //--------------------------------------------------------------------------------------------------------------
  // This is for the pressure sensor
  //--------------------------------------------------------------------------------------------------------------

  // Starts up library
  Serial.println("Initializing pressure sensor...");
  delay(1000);  // 1 second delay to give everything FLAIR
  if (!mpr.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
  } else Serial.println("Pressure sensor ready!");
  delay(1000);       // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space

  //--------------------------------------------------------------------------------------------------------------
  // This is for the MKR GPS Shield
  //--------------------------------------------------------------------------------------------------------------

  // This is to initialize the GPS Shield
  Serial.println("Initializing GPS Shield...");
  delay(1000);  // 1 second delay to give everything FLAIR
  if (!GPS.begin(GPS_MODE_SHIELD)) {
    Serial.println("Failed to initialize GPS!");
    while (1)
      ;
  } else Serial.println("GPS Ready!");
  delay(1000);       // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space

  //--------------------------------------------------------------------------------------------------------------
  // This is for the SD card board
  //--------------------------------------------------------------------------------------------------------------

  // This initializes the SD Card
  Serial.println("Initializing MKR WAN 1310 SD Card...");
  delay(1000);  // 1 second delay to give everything FLAIR
  if (!SD.begin(SD_PIN)) {
    Serial.println("Failed to initialize MKR WAN 1310 SD card!");
    while(1) delay(1);   // This waits for an SD card to be insertted
  } else Serial.println("MKR WAN 1310 SD card ready!");
  delay(1000);       // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space

  //--------------------------------------------------------------------------------------------------------------
  // This is for the Real Time Clock board
  //--------------------------------------------------------------------------------------------------------------

  Serial.println("Initializing RTC Board...");
  delay(1000);  // 1 second delay to give everything FLAIR

  // This locates the RTC board
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);  // This holds up the entire code until it can find the RTC board
  }

  rtc.start();
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //--------------------------------------------------------------------------------------------------------------
  // This is for setting up the .csv file that will act as the backup incase radio malfunction
  //--------------------------------------------------------------------------------------------------------------

  // This will get the current date fpr the file name
  DateTime now = rtc.now();
  Serial.println("Today's Date is:");
  Serial.println(String(now.day(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.year(), DEC));
  String fileName = "HAB_Flight_" + String(now.day(), DEC) + String(now.month(), DEC) + String(now.year(), DEC) + ".csv";
  delay(1000);  // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space

  // Write header to SD card
  // NOTE: The .csv filename can be a MAXIUMUM of 8 characters long. Otherwise, it just doesn't create the file. Something to do with memory restrictions
  Serial.println("Setting up .csv Data Files on SD Card...");
  delay(1000);  // 1 second delay to give everything FLAIR
  
  SD.remove("HAB_Test.csv");
  SD.remove("PhotoDat.csv");

  File BackUpData = SD.open("HAB_Test.csv", FILE_WRITE);

  if (BackUpData) {
    BackUpData.println("Date(YYYY-MM-DD),Timestamp(HH:MM:SS),Interior_Temperature_(\xB0""F),Exterior_Temperature_(\xB0""F),Pressure_(psi),Latitude(\xB0""),Longitude(\xB0""),Altitude(ft),#_of_Satellites"); // The \xB0"" prints a degree symbol
    BackUpData.close();
    Serial.println(".csv backup file created!");
    BackUpData.close();
  } else {
    Serial.println("Error constructing .csv backup file!");
  }
  delay(1000);  // 1 second delay to give everything FLAIR

  File PhotoData = SD.open("PhotoDat.csv", FILE_WRITE);

  if (PhotoData) {
    PhotoData.println("Date(YYYY-MM-DD),Timestamp(HH:MM:SS),Photo_Number,GPS_Altitude(ft)"); // The \xB0"" prints a degree symbol
    PhotoData.close();
    Serial.println(".csv photo data file created!");
    PhotoData.close();
  } else {
    Serial.println("Error constructing .csv photo file!");
  }
  delay(1000);  // 1 second delay to give everything FLAIR
  Serial.println();  // Insert space


  //--------------------------------------------------------------------------------------------------------------
  // This is for setting up the LoRa radio transmitter
  //--------------------------------------------------------------------------------------------------------------
  
  Serial.println("Initializing LoRa Module...");
  delay(1000);  // 1 second delay to give everything FLAIR

  // This is for 915 MHz Signal (United States Radio Frequency 915 MHz)
  
    // This sets up the LoRa radio
    if (!LoRa.begin(915E6)) {
      Serial.println("Starting LoRa failed!");
      while (1);
    }
  
    // This sets up some special parameters for this application in order to boost the signal of the radio (Hopefully)
    LoRa.setTxPower(20);
    LoRa.setSpreadingFactor(11);
    LoRa.setSignalBandwidth(250E3);
    LoRa.crc(); 

  // This sends the heading information for the .csv file that the reciever should be creating

    //// This is if we are sending ALL the data. Comment out if we just want the GPS data
    //LoRa.beginPacket();
    //LoRa.print("Packet,Date(YYY-MM-DD),Timestamp(HH:MM:SS),Interior_Temperature_(\xB0""F),Exterior_Temperature_(\xB0""F),Pressure_(psi),Latitude(\xB0""),Longitude(\xB0""),Altitude(ft),#_of_Satellites");
    //LoRa.endPacket();

    //// This is if we are sending JUST the GPS data. Comment out if we are sending ALL the data.
    //LoRa.beginPacket();
    //LoRa.print("Packet,Latitude(°),Longitude(°),Altitude(ft)");
    //LoRa.endPacket();

  //--------------------------------------------------------------------------------------------------------------
  // This is for printing all data to serial monitor (Comment out probably just to save computing power)
  //--------------------------------------------------------------------------------------------------------------

  Serial.println();  // Insert space
  Serial.println("START DATA...");
  Serial.println();  // Insert space
  delay(1000);       // 1 second delay to give everything FLAIR
  Serial.println("|----------------|-------------------------|-------------------------|-------------------|-----------------|------------------|------------------|-----------------|");
  Serial.println("|    Timer(s) \t |   Int_Temperature(°F)   |   Ext_Temperature(°F)   |   Pressure(psi)   |   Latitude(°)   |   Longitude(°)   |   Altitude(ft)   | # of Satellites |");
  Serial.println("|----------------|-------------------------|-------------------------|-------------------|-----------------|------------------|------------------|-----------------|");

  delay(1);  // Delay to get column labels down correctly

}

void loop() {
  //----------------------------------------------------------------------------------------------------------------
  // This is the part of the code that runs continuously
  // This loop deals with gathering and printing sensor and location data and sending it back to "base"
  // Note: This is only going to run if GPS data is available!
  //----------------------------------------------------------------------------------------------------------------

   if (GPS.available()) {

    //--------------------------------------------------------------------------------------------------------------
    // This for the GPS Shield / reads the current GPS values
    //--------------------------------------------------------------------------------------------------------------
    float Current_Latitude = GPS.latitude();    // Degrees
    float Current_Longitude = GPS.longitude();  // Degrees
    float Current_Altitude = GPS.altitude();    // Meters
    float Current_Speed = GPS.speed();          // Kilometers/Hour
    float Satellites = GPS.satellites();          // Just number of satellites pinging to GPS

    //--------------------------------------------------------------------------------------------------------------
    // This is for the temperature sensors / reads current temperatures
    //--------------------------------------------------------------------------------------------------------------
    sensors.requestTemperatures();

    //--------------------------------------------------------------------------------------------------------------
    // This is for the pressure sensor / reads current pressure
    //--------------------------------------------------------------------------------------------------------------
    mpr.readPressure();

    //--------------------------------------------------------------------------------------------------------------
    // This is for the RTC chip / reads current date and time
    //--------------------------------------------------------------------------------------------------------------
    DateTime now = rtc.now();

    //----------------------------------------------------------------------------------------------------------------
    // This is for printing all data to the backup .csv file on the SD card
    //----------------------------------------------------------------------------------------------------------------

    //String fileName = "HAB_Flight_" + String(now.day(), DEC) + String(now.month(), DEC) + String(now.year(), DEC) + ".csv";
    File BackUpData = SD.open("HAB_Test.csv", FILE_WRITE);

    if (BackUpData) {
      BackUpData.print(String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC));
      BackUpData.print(",");
      BackUpData.print(String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC));
      BackUpData.print(",");
      BackUpData.print(sensors.getTempF(InsideThermometer));
      BackUpData.print(",");
      BackUpData.print(sensors.getTempF(OutsideThermometer));
      BackUpData.print(",");
      BackUpData.print(mpr.readPressure() / 68.947572932);
      BackUpData.print(",");
      BackUpData.print(Current_Latitude, 7);
      BackUpData.print(",");
      BackUpData.print(Current_Longitude, 7);
      BackUpData.print(",");
      BackUpData.print(Current_Altitude / 3.280839895);
      BackUpData.print(",");
      BackUpData.println(Satellites);
      BackUpData.close();
    } else {
      Serial.println("Error Opening Backup Data File...");
    }

    //----------------------------------------------------------------------------------------------------------------
    // This is for printing all data to the serial monitor for lab testing
    // Note: Can and probably should comment this out before actually launching balloon
    //----------------------------------------------------------------------------------------------------------------

    Serial.print("|\t");
    Serial.print(millis() / 1000);
    Serial.print("\t |     \t");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(sensors.getTempF(InsideThermometer));  // Fahrenheit
    Serial.print("          | \t ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(sensors.getTempF(OutsideThermometer));  // Fahrenheit
    Serial.print("          | \t ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(mpr.readPressure() / 68.947572932);  // psi
    Serial.print("       | ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(Current_Latitude, 7);  // Degrees
    Serial.print("   | ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(Current_Longitude, 7);  // Degrees
    Serial.print("   | ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(" ");
    Serial.print(Current_Altitude / 3.280839895);  // ft
    Serial.print("       | \t ");
    Serial.print(" ");
    Serial.print(Satellites);  // Amount
    Serial.print("        | ");

    Serial.println();  // Begin new line

    //----------------------------------------------------------------------------------------------------------------
    // This is for sending data via LoRa
    //----------------------------------------------------------------------------------------------------------------

      // This sends ALL the data. Comment out if just want GPS position
      LoRa.beginPacket();
      LoRa.print("BLAW426");
      LoRa.print(",");
      LoRa.print("Packet ");
      LoRa.print(counter);
      LoRa.print(",");
      LoRa.print(String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC));
      LoRa.print(",");
      LoRa.print(String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC));
      LoRa.print(",");
      LoRa.print(sensors.getTempF(InsideThermometer));
      LoRa.print(",");
      LoRa.print(sensors.getTempF(OutsideThermometer));
      LoRa.print(",");
      LoRa.print(mpr.readPressure() / 68.947572932);
      LoRa.print(",");
      LoRa.print(Current_Latitude, 7);
      LoRa.print(",");
      LoRa.print(Current_Longitude, 7);
      LoRa.print(",");
      LoRa.print(Current_Altitude / 3.280839895);
      LoRa.print(",");
      LoRa.print(Satellites);
      LoRa.endPacket();

      //// This sends JUST the GPS data. Comment out if want ALL the data.
      //LoRa.beginPacket();
      //LoRa.print("Packet ");
      //LoRa.print(counter);
      //LoRa.print(",");
      //LoRa.print(Current_Latitude, 7);
      //LoRa.print(",");
      //LoRa.print(Current_Longitude, 7);
      //LoRa.print(",");
      //LoRa.print(Current_Altitude / 3.280839895);
      //LoRa.endPacket();

    // This increases tha packet counter up 1
    counter++;

    //----------------------------------------------------------------------------------------------------------------
    // This is for telling the camera its time to take a picture
    //----------------------------------------------------------------------------------------------------------------

    if(millis() > Next_Picture_At){
      Serial.println("Sent Photo Request...");

      File PhotoData = SD.open("PhotoDat.csv", FILE_WRITE);

      if (PhotoData) {

      PhotoData.print(String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC));
      PhotoData.print(",");
      PhotoData.print(String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC));
      PhotoData.print(",");
      PhotoData.print(String(photoNumber));
      PhotoData.print(",");
      PhotoData.println(Current_Altitude / 3.280839895);
      PhotoData.close();
      }
      else {
        Serial.println("Error Opening Backup Data File...");
      }

      digitalWrite(Trigger_PIN, HIGH);
      delay(100);
      digitalWrite(Trigger_PIN, LOW);

      Next_Picture_At = millis() + Picture_Interval_MS;

      ++photoNumber;
      

    }

      //// Delay to keep things running smoothly
      //delay(2000);    // 5 Seconds


  }
}
