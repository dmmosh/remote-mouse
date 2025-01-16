#include "header.h"
// variables for the bluetooth server and hid device
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLEAdvertising *pAdvertising;
BLEServer *pServer;
BLEScan* pBLEScan;
BLEMouse* mouse;
bool connected =false;





void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    pinMode(LED,OUTPUT);
    
    digitalWrite(LED,ON);
    
    mouse->begin();

    //ESP_LOGD(LOG_TAG, "Advertising started!");
    //delay(portMAX_DELAY);
}


void loop() {

    if(!connected){
      pAdvertising->start();
      connect_wait();
    }  
    mouse->move(1,1);
    vTaskDelay(10/portTICK_PERIOD_MS);
}