//--------------------------------------------------------------------------------------------------------------
// These are all the libraries that need to be loaded in for this code to run
//--------------------------------------------------------------------------------------------------------------

#include <SPI.h>
#include <LoRa.h> 
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

int LoRaBytes;
char LoRaData[256];

//--------------------------------------------------------------------------------------------------------------
// This is for the mini OLED display
//--------------------------------------------------------------------------------------------------------------

// This is to define the OLED display width and height in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// This is to declare that the screen (SSD1306) is connected to I2C (SDA and SCL pins)
#define OLED_RESET -1  // This is the reset pin# (-1 denotes its sharing the Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//--------------------------------------------------------------------------------------------------------------
// This is for the SD card 
//--------------------------------------------------------------------------------------------------------------

// Define the pins for the camera and SD card
#define SD_PIN      7           // This is the pin that the CS leg of thecamera is connected to

// This creates a File object to store the file
File PacketData;

void setup() {
  //-------------------------------------------------------
  // This will set up the serial monitor
  //-------------------------------------------------------
  Serial.begin(9600);
  while (!Serial);

  //-------------------------------------------------------
  // This will set up the LoRa Reciever
  //-------------------------------------------------------

  Serial.println("Starting LoRa Receiver...");

  // This is for 433 MHz Signal (Australian Radio Frequency 915 MHz)
  
    // This sets up the LoRa radio
    if (!LoRa.begin(433E6)) {
      Serial.println("Starting LoRa failed!");
      while (1);
    }
  
    // This sets up some special parameters for this application in order to boost the signal of the radio (Hopefully)
    LoRa.setTxPower(20);
    LoRa.setSpreadingFactor(11);
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.crc();  

  Serial.println();
  delay(1000);  // 1 second delay to give everything FLAIR

  //-------------------------------------------------------
  // This will set up SD card to print packets to
  //-------------------------------------------------------

  // This initializes the SD Card
    Serial.println("Initializing MKR WAN 1310 SD Card...");
    delay(1000);  // 1 second delay to give everything FLAIR
    if (!SD.begin(SD_PIN)) {
      Serial.println("Failed to initialize MKR WAN 1310 SD card!");
      while(1) delay(1);   // This waits for an SD card to be insertted
    } else Serial.println("MKR WAN 1310 SD card ready!");
    delay(1000);       // 1 second delay to give everything FLAIR
    Serial.println();  // Insert space
  
  // This removes data on the SD card if it exists
  Serial.println("Setting up .csv Data Files on SD Card...");
  delay(1000);  // 1 second delay to give everything FLAIR

  SD.remove("PackData.csv"); 

  // This creates a new Packet Data file
  File PacketData = SD.open("PackData.csv", FILE_WRITE);

  if (PacketData) {
    PacketData.println("Packet,Date(YYYY-MM-DD),Timestamp(HH:MM:SS),Interior_Temperature_(\xB0""F),Exterior_Temperature_(\xB0""F),Pressure_(psi),Latitude(\xB0""),Longitude(\xB0""),Altitude(ft),#_of_Satellites");
    PacketData.close();
    Serial.println(".csv file created!");
  } else {
    Serial.println("Error constructing .csv file!");
  }

  // This helps parse the data for the SD Card
  int LoRaBytes;
  char LoRaData[256];
  
  Serial.println();
  delay(1000);  // 1 second delay to give everything FLAIR

  //-------------------------------------------------------
  // This will set up the mini OLED display to display the current latitude/longitude
  //-------------------------------------------------------

  Serial.println("Initializing OLED Screen...");
  delay(1000);  // 1 second delay to give everything FLAIR

  // This turns on the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // This will start up the display with Adafruit splashscreen
  display.display();
  delay(2000);  // Give it 2 seconds to do its thing

  // This will clear the splashscreen
  display.clearDisplay();
  display.setTextColor(WHITE);

  Serial.println("OLED Screen Initialized!");

  Serial.println();
  delay(1000);  // 1 second delay to give everything FLAIR

  //-------------------------------------------------------
  // Let the games begin
  //-------------------------------------------------------

  Serial.println("Data Collection Started...");
  Serial.println();
  delay(1000);  // 1 second delay to give everything FLAIR
  
}

void loop() {
  
  //-------------------------------------------------------
  // This will try and parse a packet that it recieved
  //-------------------------------------------------------
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {

    // This will read the Packet
    while (LoRa.available()) {

      //Serial.print((char)LoRa.read());
      LoRaBytes = LoRa.readBytes(LoRaData, sizeof(LoRaData) - 1);
      LoRaData[LoRaBytes] = '\0';  // Null-terminate the string

    }
      
      // This prints the data to the serial monitor
      Serial.println(LoRaData);

    // This should hopefully print the latitude, longitude, and altitude to the mini OLED screen

    String LoRaDataString = String(LoRaData);

    String tokens[10];
    int index = 0;
    int startPosition = 0;
    int endPosition = LoRaDataString.indexOf(',');

    while (endPosition != -1) {
      tokens[index] = LoRaDataString.substring(startPosition, endPosition);
      startPosition = endPosition + 1;
      endPosition = LoRaDataString.indexOf(',',startPosition);
      index++;

      // Extract out latitude and longitude information
      float latitude = tokens[7].toFloat();
      float longitude = tokens[8].toFloat();
      float altitude = tokens[9].toFloat();

      // This is for displaying the latitude, longitude, and altitude to the OLED Screen
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.print("Latitude: ");
      display.setTextSize(2);
      display.setCursor(0,10);
      display.print(latitude, 6);
      display.setTextSize(1);
      display.setCursor(0,26);
      display.print("Longitude: ");
      display.setTextSize(2);
      display.setCursor(0,36);
      display.println(longitude, 6);
      display.setTextSize(1);
      display.setCursor(0,53);
      display.print("Altitude:");
      display.print(altitude,0);
      display.print("ft");

      display.display();

    }

    // This is for saving each packet data to the SD card
    File PacketData = SD.open("PackData.csv", FILE_WRITE);
    PacketData.print(LoRaDataString);
    PacketData.println();
    PacketData.close();

  }

}
