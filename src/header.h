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
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_hidd_api.h"
#include "esp_bt_device.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"




// pins 
#define LED 2
#define VRX 13
#define VRY 12
#define SW 14
#define JOYSTICK 27


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
extern const char *TAG;
extern bool connected;
extern int16_t x, y;
extern uint8_t click;


void connect_wait();
void move(uint8_t click,uint8_t x , uint8_t y);
void move(uint8_t click,uint8_t x );
void move(uint8_t click);
bool str_equals(const char* str1, const char* str2);



#endif