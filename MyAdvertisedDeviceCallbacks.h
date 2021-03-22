#ifndef MYADVERTISEDDEVICECALLBACKS_H_
#define MYADVERTISEDDEVICECALLBACKS_H_
#include <BLEScan.h>
#include "BLEBeacon.h"
#include <BLEAdvertisedDevice.h>
#include <string>
using namespace std;
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
};

#endif
