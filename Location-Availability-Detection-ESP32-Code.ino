
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <string>
//#include <WiFi.h>
#include "BLEBeacon.h"
using namespace std;
int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
              BLEBeacon oBeacon = BLEBeacon();
              
              std::string strManufacturerData = advertisedDevice.getManufacturerData();
              uint8_t cManufacturerData[100];
              strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
              
              oBeacon.setData(strManufacturerData);
              String proximityUUID = getProximityUUIDString(oBeacon);

              if(proximityUUID == "fa9dbcd1e23948dfa7f6d7e97237253e")
              {
                    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
                    Serial.printf("=> %d \n", advertisedDevice.getRSSI());
                    Serial.printf("=> %s \n\n", proximityUUID.c_str()); 
              }
      

    }
    
    String getProximityUUIDString(BLEBeacon beacon) {
      std::string serviceData = beacon.getProximityUUID().toString().c_str();
      int serviceDataLength = serviceData.length();
      String returnedString = "";
      int i = serviceDataLength;
      while (i > 0)
      {
        if (serviceData[i-1] == '-') {
          i--;
        }
        char a = serviceData[i-1];
        char b = serviceData[i-2];
        returnedString += b;
        returnedString += a;
    
        i -= 2;
      }
    
      return returnedString;
    }
};

const char* ssid = "stuff";
const char* password = "shit";

void setup() {
  
  Serial.begin(115200);
  Serial.println("Scanning...");
/*

  //wifi shit
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
*/
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  //Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
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
