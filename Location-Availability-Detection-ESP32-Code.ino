
#include "SensusDevice.h"
using namespace std;
int scanTime = 5; //In seconds


const char* ssid = "GNEGINNOV32";
const char* password = "testinnov";
const char* mqtt_server = "broker.hivemq.com";

SensusDevice sensusDevice;
void setup() {
  
  Serial.begin(115200);
  sensusDevice.connectToWifi(ssid, password);
  sensusDevice.setupMQTTClient(mqtt_server);
  sensusDevice.setupBLEScanner();
}

void loop() {
  sensusDevice.detectDevices();
}
