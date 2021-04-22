/*
* Author: Mohamad Ariffin Zulkifli
* Organization: Myinvent Technologies Sdn Bhd
*
* This sketch has 3 execution steps:
* 1. Initialized WiFi connectivity
* 2. Connecting to WiFi and MQTT broker
* 3. Wait and execute incoming command
* 
* Please select ESP32/ESP8266 Boards before compiling the sketch
* (example) Go to menu, Tools > Board > ESP32 Arduino or ESP8266 Boards
*
*/

#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[]     = "YourWiFiSSID";       // replace with your WiFi SSID
const char password[] = "YourWiFiPassword";   // replace with your WiFi password

const char clientId[] = "YourClientId";       // replace with your MQTT Client Id
const char topic[]    = "YourMqttTopic";      // replace with your MQTT Topic
const char server[]   = "broker.hivemq.com";  // replace with your MQTT Broker

WiFiClient net;
MQTTClient client;

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
  while (!client.connect(clientId)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

  client.subscribe(topic);
}

// Sub function for incoming message from topic subscription
void messageReceived(String &topic, String &payload) {
  
  Serial.println("Received command: " +  payload);
  
  // D7,1
  String pin    = payload.substring(0, payload.indexOf(","));
  String state  = payload.substring(payload.indexOf(",")+1);

  if(pin == "D6") digitalWrite(D6, state.toInt());
  if(pin == "D7") digitalWrite(D7, state.toInt());
}

void setup() {
  Serial.begin(9600);

  // STEP 1 - Initialized WiFi connectivity
  WiFi.begin(ssid, password);

  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  client.begin(server, net);
  client.onMessage(messageReceived);

  // STEP 2 - Connecting to WiFi and MQTT broker
  connect();
}

void loop() {
  // STEP 3 - Wait and execute incoming command on messageReceived() function
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) connect();
}
