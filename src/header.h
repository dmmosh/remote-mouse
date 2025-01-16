#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>


// // BLE LIBRARIES / DATA 
// #include <BLEDevice.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>
// #include <BLEClient.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLEAdvertising.h>
// #define SERVICE_UUID "9f46b94c-9574-4f6c-bd1b-ddc3a7a83a43"
// #define CHARACTERISTIC_UUID "afe8ef56-902f-4b38-a6a2-0eade0aca572"
// bool deviceConnected = false;
// BLEScan *scan;


#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <driver/adc.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"


// pins 
#define LED 2
#define VRX 13
#define VRY 12
#define SW 14


#define OFF 0
#define ON 1

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02
#define MOUSE_ID 0x03

#define PERIOD 20 // period (in ms)

// MOUSE IDS
#define CLICK_LEFT 1
#define CLICK_RIGHT 2
#define CLICK_MIDDLE 4
#define CLICK_BACK 8
#define CLICK_FORWARD 16



// variables for the bluetooth server and hid device
extern BLEHIDDevice* hid;
extern BLECharacteristic* input;
extern BLECharacteristic* output;
extern BLEAdvertising *pAdvertising;
extern BLEServer *pServer;
extern BLEScan* pBLEScan;
extern bool connected;
extern const uint8_t mouse_report_desc[];


void connect_wait();
void move(uint8_t x =0, uint8_t y=0, uint8_t xwheel=0, uint8_t ywheel=0);
bool str_equals(const char* str1, const char* str2);

void my_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gatts_cb_param_t* param);

class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
    connected = true;
    Serial.println("Connected");
    // NEEDED ACTIONScdjknckj
	BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  	desc->setNotifications(true);
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
    Serial.println("Disconnect");
	BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  	desc->setNotifications(false);		
  }
};



#endif