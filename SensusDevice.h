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

using namespace std;
class SensusDevice {
    public:
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

        void detectDevices()
        {
              BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
              Serial.print("Devices found: ");
              Serial.println(getCountOfPhones(foundDevices));
              //Serial.println(foundDevices.getCount());
              //Serial.println("Scan done!");
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
            //if(proximityUUID == "fa9dbcd1e23948dfa7f6d7e97237253e")
            if(proximityUUID != "00000000000000000000000000000000")
            {
                    count ++;
                    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
                    Serial.printf("=> %d \n", advertisedDevice.getRSSI());
                    Serial.printf("=> %s \n\n", proximityUUID.c_str()); 
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
    
        BLEScan* pBLEScan;
        WiFiClient espClient;
        PubSubClient *client;
        int scanTime = 5;
        int maxCapacity;
};

#endif
