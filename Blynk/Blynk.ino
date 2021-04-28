#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "YOUR_AUTH_TOKEN";

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

#include <SimpleDHT.h>

int pinDHT11 = D1;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  byte temperature = 0;
  byte humidity = 0;
  
  int err = SimpleDHTErrSuccess;
  
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(",");
    Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

  Blynk.virtualWrite(V0, (int)temperature);
  Blynk.virtualWrite(V1, (int)humidity);

  delay(1500);
}
