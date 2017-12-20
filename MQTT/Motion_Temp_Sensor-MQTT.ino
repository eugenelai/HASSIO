/**
 * Example for reading temperature and humidity
 * using the DHT22 and ESP8266
 *
 * Copyright (c) 2016 Losant IoT. All rights reserved.
 * https://www.losant.com
 */

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "24L_Centre_Hollywood_2.4G"
#define wifi_password "37113000"

#define mqtt_server "m13.cloudmqtt.com"
#define mqtt_user "zsgfuwpg"
#define mqtt_password "wpUR9WgHR1Xm"

WiFiClient espClient;
PubSubClient client(espClient);

#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
#define motion_topic "sensor/motion"

#define DHTPIN 13     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
int motionSensor = 12;  // Digital pin D6

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  pinMode(motionSensor, INPUT);   // declare sensor as input
  dht.begin();
  
  setup_wifi();
  client.setServer(mqtt_server, 18745);

  // Wait for serial to initialize.
  while(!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Sensor Module!");
  Serial.println("-------------------------------------");

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

bool checkMotion(long newValue, long prevValue){
  return newValue != prevValue;  
}

int timeSinceLastRead = 0;
long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
long motion = LOW;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  // Report every 2 seconds.
  if(timeSinceLastRead > 500) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float newHum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float newTemp = dht.readTemperature();
    long newMotion = digitalRead(motionSensor);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(newHum) || isnan(newTemp)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("New temperature: ");
      Serial.println(String(temp).c_str());
      client.publish(temperature_topic, String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity: ");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }
   
    if (checkMotion(newMotion, motion)){
      motion = newMotion;
      Serial.print("Motion sensor: ");
      Serial.println(String(motion).c_str());
      client.publish(motion_topic, String(motion).c_str(), true);
      }
      
    
    
//    Serial.print("Humidity: ");
//   Serial.print(newHum);
//    Serial.print(" %\t");
//    Serial.print("Temperature: ");
//    Serial.print(newTemp);
//    Serial.print(" *C ");

//    long state = digitalRead(sensor);
    
//    if(state == HIGH) {
//      Serial.println("Motion detected!");
//    }
//    else {
//      Serial.println("Motion absent!");
//    }

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}
