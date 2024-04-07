#include "esp_camera.h"         // Library for Camera/ESP32-CAM Board
#include "Arduino.h"            // Library for Arduino
#include "SD_MMC.h"             // Library for microSD Card
#include "FS.h"                 // Library for microSD Card
#include "soc/soc.h"            // Disable brownout problems
#include "soc/rtc_cntl_reg.h"   // Disable brownout problems
  
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Define Trigger Pin
#define Trigger_PIN       1

const char * photoPrefix = "/Photo_";
int photoNumber = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // This waits for the serial port to connect
  }
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 15;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 15;
    config.fb_count = 2;
  }

  #if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  #endif

  // camera init
  esp_err_t err = esp_camera_init( & config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s -> id.PID == OV3660_PID) {
    s -> set_vflip(s, 1); // flip it back
    s -> set_brightness(s, 1); // up the brightness just a bit
    s -> set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s -> set_framesize(s, FRAMESIZE_UXGA);

  #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s -> set_vflip(s, 1);
  s -> set_hmirror(s, 1);
  #endif

  Serial.println("Initialising Camera SD card");
  if (!SD_MMC.begin()) {
    Serial.println("Failed to initialise Camea SD card!");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Camera SD card slot appears to be empty!");
    return;
  }

  pinMode(Trigger_PIN, INPUT);

}

void loop() {

  if (digitalRead(Trigger_PIN) == HIGH) {

    Capture_and_Save_Photo();

  }

  delay(100);
  
}

void Capture_and_Save_Photo() {

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  String photoFileName = photoPrefix + String(photoNumber) + ".jpg";
  fs::FS & fs = SD_MMC;
  Serial.printf("Picture file name: %s\n", photoFileName.c_str());

  File file = fs.open(photoFileName.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in writing mode");
  } else {
    file.write(fb -> buf, fb -> len);
    Serial.printf("Saved file to path: %s\n", photoFileName.c_str());
    ++photoNumber;
  }
  file.close();
  esp_camera_fb_return(fb);

}