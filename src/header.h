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



#define LED 2
#define OFF 0
#define ON 1

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02
#define MOUSE_ID 0x03


#define HID_SERVICE_UUID        0x1812
#define HID_REPORT_UUID         0x2A4D
#define HID_REPORT_DESC_UUID    0x2A4B

// MOUSE IDS
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_BACK 8
#define MOUSE_FORWARD 16



// variables for the bluetooth server and hid device
extern BLEHIDDevice* hid;
extern BLECharacteristic* input;
extern BLECharacteristic* output;
extern BLEAdvertising *pAdvertising;
extern BLEServer *pServer;
extern BLEScan* pBLEScan;
extern bool connected;


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

uint8_t mouse_report[] = {0x00, 0x00, 0x00, 0x00};  // Report for moving cursor (X, Y)

// HID Report Descriptor (for Mouse)
uint8_t mouse_report_desc[] = {
  0x05, 0x01,      // Usage Page (Generic Desktop)
  0x09, 0x02,      // Usage (Mouse)
  0xa1, 0x01,      // Collection (Application)
  0x09, 0x01,      // Usage (Pointer)
  0xa1, 0x00,      // Collection (Physical)
  0x05, 0x09,      // Usage Page (Buttons)
  0x19, 0x01,      // Usage Minimum (Button 1)
  0x29, 0x03,      // Usage Maximum (Button 3)
  0x15, 0x00,      // Logical Minimum (0)
  0x25, 0x01,      // Logical Maximum (1)
  0x75, 0x01,      // Report Size (1)
  0x95, 0x03,      // Report Count (3)
  0x81, 0x02,      // Input (Data, Variable, Absolute)
  0x95, 0x01,      // Report Count (1)
  0x75, 0x05,      // Report Size (5)
  0x81, 0x03,      // Input (Constant)
  0x05, 0x01,      // Usage Page (Generic Desktop)
  0x09, 0x30,      // Usage (X Axis)
  0x09, 0x31,      // Usage (Y Axis)
  0x15, 0x81,      // Logical Minimum (-127)
  0x25, 0x7f,      // Logical Maximum (127)
  0x75, 0x08,      // Report Size (8)
  0x95, 0x02,      // Report Count (2)
  0x81, 0x06,      // Input (Data, Variable, Relative)
  0xc0,            // End Collection (Physical)
  0xc0             // End Collection (Application)
};


#endif