/*
* Author: Mohamad Ariffin Zulkifli
* Organization: Myinvent Technologies Sdn Bhd
*
* This sketch has 3 execution steps:
* 1. Initialized WiFi connectivity
* 2. Connecting to WiFi and MQTT broker
* 3. Sensors reading for data acquisition
* 4. Send data to MQTT topic
* 
* Please select ESP32/ESP8266 Boards before compiling the sketch
* (example) Go to menu, Tools > Board > ESP32 Arduino or ESP8266 Boards
*
*/

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <SimpleDHT.h>

const char ssid[]     = "YourWiFiSSID";       // replace with your WiFi SSID
const char password[] = "YourWiFiPassword";   // replace with your WiFi password

const char clientId[] = "YourClientId";       // replace with your MQTT Client Id
const char topic[]    = "YourMqttTopic";      // replace with your MQTT Topic
const char server[]   = "broker.hivemq.com";  // replace with your MQTT Broker

SimpleDHT22 dht22(D8);

WiFiClient net;
MQTTClient mqtt;

unsigned long lastMillis = 0;

// Sub-function to connect to WiFi and MQTT broker
void connect() {
  
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

  Serial.print("Connecting to MQTT broker ...");
  while (!mqtt.connect(clientId)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");
}

// Sub function for incoming message from topic subscription
void messageReceived(String &topic, String &payload) {
  
}

void setup() {
  Serial.begin(9600);

  // STEP 1 - Initialized WiFi connectivity
  WiFi.begin(ssid, password);

  mqtt.begin(server, net);
  mqtt.onMessage(messageReceived);

  // STEP 2 - Connecting to WiFi and MQTT broker
  connect();
}

void loop() {
  mqtt.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqtt.connected()) connect();

  // STEP 3 - Sensors reading for data acquisition
  float suhu = 0;
  float kelembapan = 0;
  
  int err = SimpleDHTErrSuccess;
  if ((err = dht2.read2(&suhu, &kelembapan, NULL)) != SimpleDHTErrSuccess) {
    return;
  }

  // update data interval to Favoriot data stream using millis() function
  if(millis() - lastMillis > 5000){
    lastMillis = millis();

    // STEP 4 - Send data to MQTT topic
    mqtt.publish(topic, suhu + "," + kelembapan);
  }
}
