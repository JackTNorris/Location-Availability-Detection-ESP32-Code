#ifndef SENSUSDEVICE_H_
#define SENSUSDEVICE_H_
#include <WiFi.h>
#include <BLEScan.h>
#include "BLEBeacon.h"
#include <BLEDevice.h>

#include "MyAdvertisedDeviceCallbacks.h"
#include <BLEAdvertisedDevice.h>
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
              //Serial.println(foundDevices.getCount());
              //Serial.println("Scan done!");
              pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
              delay(2000);
        }
    private:
        BLEScan* pBLEScan;
        int scanTime = 5;

};

#endif
