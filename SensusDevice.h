#ifndef SENSUSDEVICE_H_
#define SENSUSDEVICE_H_
#include <WiFi.h>
#include <BLEScan.h>
#include "BLEBeacon.h"
#include <BLEDevice.h>
#include "MyAdvertisedDeviceCallbacks.h"
#include <BLEAdvertisedDevice.h>
#include <PubSubClient.h>
#include <string>
#include <sstream>

using namespace std;
class SensusDevice {
    public:
        SensusDevice(): ROOM_KEY("-MUCHPqgtcwfCLoY386e"), ORGANIZATION("uark") {}
    
        void connectToWifi(const char *ssid, const char *password)
        {
            Serial.println("Scanning...");
            WiFi.begin(ssid, password);
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.println("Connecting to WiFi..");
            }
            Serial.println("Connected to Wifi Network");
        };

  
        void setupBLEScanner()
        {
            BLEDevice::init("");
            pBLEScan = BLEDevice::getScan(); //create new scan
            pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
            pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
            pBLEScan->setInterval(100);
            pBLEScan->setWindow(99);  // less or equal setInterval value
        }

        void sendUpdatedOccupancyInfo()
        {
            if (!client->connected()) {
              reconnectMQTT();
            }
            client->loop();
            string payload = "{\"key\": \"" + ROOM_KEY + "\", \"organization\": \"" + ORGANIZATION + "\", \"occupancy\": " + occupancyString() + "  }";
            client->publish("gnegTest", payload.c_str());
        }

        void detectDevices()
        {
              BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
              //Serial.print("Devices found: ");
              int countOfPhones = getCountOfPhones(foundDevices);
              if(countOfPhones != occupancy)
              {
                occupancy = countOfPhones;
                sendUpdatedOccupancyInfo(); 
                Serial.println("Updated info");
              }
              Serial.println(getCountOfPhones(foundDevices));
              pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
              delay(2000);
        }

        void setupMQTTClient(const char *server)
        {
          client = new PubSubClient(espClient);
          client->setServer(server, 1883);
        }
        
        ~SensusDevice()
        {
            delete pBLEScan;
            delete client;
        }
        
    private:
        int getCountOfPhones(BLEScanResults foundDevices)
        {
          int count = 0;
          int totalDevices = foundDevices.getCount();
          for(int i = 0; i < totalDevices; i++)
          {
            BLEAdvertisedDevice advertisedDevice = foundDevices.getDevice(i);
            BLEBeacon oBeacon = BLEBeacon();
            
            std::string strManufacturerData = advertisedDevice.getManufacturerData();
            uint8_t cManufacturerData[100];
            strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
            
            oBeacon.setData(strManufacturerData);
            String proximityUUID = getProximityUUIDString(oBeacon);
            String sensusKey = proximityUUID.substring(proximityUUID.length() - 12);
            //if(proximityUUID == "fa9dbcd1e23948dfa7f6-d7e97237253e")
            if(sensusKey == "73656e737573") //73656e737573 is "sensus" encoded
            {
                    count ++;
            }
          }
          return count;
        }

        String getProximityUUIDString(BLEBeacon beacon) 
        {
          string serviceData = beacon.getProximityUUID().toString().c_str();
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

        string occupancyString()
        {
            stringstream ss;
            ss << occupancy;
            string out_string = ss.str();
            return out_string;
        }

        void reconnectMQTT() {
            // Loop until we're reconnected
            while (!client->connected()) {
              Serial.print("Attempting MQTT connection...");
              // Attempt to connect
              if (client->connect("")) {
                Serial.println("connected");
              } else {
                Serial.print("failed, rc=");
                Serial.print(client->state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
              }
            }
        }


        BLEScan* pBLEScan;
        WiFiClient espClient;
        PubSubClient *client;
        int scanTime = 5;
        int occupancy = 0;
        const string ROOM_KEY;
        const string ORGANIZATION;
        
};

#endif
