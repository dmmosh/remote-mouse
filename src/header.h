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
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>


#define LED 2
#define OFF 0
#define ON 1

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02
#define MOUSE_ID 0x03

// variables for the bluetooth server and hid device
extern BLEHIDDevice* hid;
extern BLECharacteristic* input;
extern BLECharacteristic* output;
extern BLEAdvertising *pAdvertising;
extern BLEServer *pServer;
extern BLEScan* pBLEScan;
extern bool connected = false;


inline void connect_wait();
bool str_equals(const char* str1, const char* str2);

void setup();
void loop();



#endif