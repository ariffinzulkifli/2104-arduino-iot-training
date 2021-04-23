/*
* Author: Mohamad Ariffin Zulkifli
* Organization: Myinvent Technologies Sdn Bhd
*
* This sketch has 3 execution steps:
* 1. Initialized Favoriot connectivity
* 2. Sensors reading for data acquisition
* 3. Send data to Favoriot Data Stream
* 
* Please select ESP32/ESP8266 Boards before compiling the sketch
* (example) Go to menu, Tools > Board > ESP32 Arduino or ESP8266 Boards
*
*/

#include <FavoriotHTTP.h>
#include <SimpleDHT.h>

const char ssid[]     = "YourWiFiSSID";           // replace with your WiFi SSID
const char password[] = "YourWiFiPassword";       // replace with your WiFi password
const char apikey[]   = "YourDeviceAccessToken";  // replace with your Favoriot Device Access Token

FavoriotHTTP favoriot;
SimpleDHT11 dht11(D8);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);

  // STEP 1 - Initialized Favoriot connectivity
  favoriot.begin(ssid, password, apikey);
}

void loop() {
  
  // STEP 2 - Sensors reading for data acquisition
  byte suhu = 0;
  byte kelembapan = 0;
  
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&suhu, &kelembapan, NULL)) != SimpleDHTErrSuccess) {
    return;
  }

  // update data interval to Favoriot data stream using millis() function
  if(millis() - previousMillis > 5000){
    previousMillis = millis();

    // STEP 3 - Send data to Favoriot data dtream
    favoriot.deviceId("YourDeviceDeveloperId"); // replace with your Device Developer ID
    // declaring key and value of data stream
    favoriot.dataStream("suhu", String(suhu));
    favoriot.dataStream("kelembapan", String(kelembapan));
    // end of data stream
    favoriot.dataStreamEnd();

  }
}
