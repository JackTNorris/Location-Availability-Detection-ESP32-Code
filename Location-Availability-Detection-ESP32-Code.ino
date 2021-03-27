/*
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <string>
#include <WiFi.h>
#include "BLEBeacon.h"
*/
#include "SensusDevice.h"
using namespace std;
int scanTime = 5; //In seconds


const char* ssid = "GNEGINNOV32";
const char* password = "testinginnov";
SensusDevice sensusDevice;
void setup() {
  
  Serial.begin(115200);
  //sensusDevice.connectToWifi(ssid, password);
  sensusDevice.setupBLEScanner();
}

void loop() {
  sensusDevice.detectDevices();
}





//for later
/*
 #include "WiFi.h"
#include <PubSubClient.h>

const char* ssid = "GNEGINNOV32";
const char* password =  "testinginnov";

const char* mqtt_server = "broker.hivemq.com";

long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("")) {
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


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
boolean f = true;
void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000 && f) {
    //lastMsg = now;
    //client.publish("esp32/temperature", tempString);
    client.publish("gnegTest", NULL);
    f = false;
  }
}
 */
