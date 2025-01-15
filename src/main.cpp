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


// variables for the bluetooth server and hid device
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLEAdvertising *pAdvertising;
BLEServer *pServer;
BLEScan* pBLEScan;


bool connected = false;



void rgb_lamp(void* args);
void lamp(const uint8_t new_state);
inline void connect_wait();
bool str_equals(const char* str1, const char* str2);



class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
    connected = true;
    Serial.println("Connected");
    // NEEDED ACTIONScdjknckj
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
    Serial.println("Disconnect");
  }
};



inline void connect_wait(){
    uint16_t ms_5  = 0;
    uint16_t seconds = 0;
    pinMode(LED,OUTPUT);

    while(!connected){
        
        if(ms_5 >= 200){    
            if(seconds<21){ // note: at 20 second mark, it sets led to LOW and it stays there afterwards (to save power and not be annoying)
                digitalWrite(LED,seconds%2);
            }

            Serial.printf("Waiting for device to pair... %is\n", seconds);
            seconds++;
            ms_5 = 0;
        } else {
        ms_5++;
        }
        vTaskDelay(5/portTICK_PERIOD_MS);
    }
}

bool str_equals(const char* str1, const char* str2){
    uint8_t j = 0;
    while(str1[j] != '\0' || str2[j] != '\0'){
        if (str1[j] != str2[j]){
            return false;
        }
        j++;
    }
    return true;
}



void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(LED,OUTPUT);
  BLEDevice::init("Remote Mouse");
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); 
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());
  pServer->getPeerDevices(false);

  hid = new BLEHIDDevice(pServer);
  input = hid->inputReport(1); // <-- input REPORTID from report map
  output = hid->outputReport(1); // <-- output REPORTID from report map

    std::string name = "Phone Lamp";    
  hid->manufacturer()->setValue(name);

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00,0x02);

  BLESecurity *pSecurity = new BLESecurity();
  //  pSecurity->setKeySize();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    hid->startServices();

    pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_BARCODE);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();
    hid->setBatteryLevel(7);

    pBLEScan = BLEDevice::getScan();  
    pBLEScan->setActiveScan(true);  // Set active scan to get more information (e.g., RSSI)
    pBLEScan->setInterval(100);     // Set scan interval (in milliseconds)
    pBLEScan->setWindow(99);        // Set scan window (in milliseconds)
    
    

    //ESP_LOGD(LOG_TAG, "Advertising started!");
    //delay(portMAX_DELAY);
}


void loop() {

  if(!connected){
    lamp(OFF);
    pAdvertising->start();
    connect_wait();
  }  
    digitalWrite(LED,ON);

    // std::map<uint16_t, conn_status_t> devices = pServer->getPeerDevices(false);

    // for(const auto& pair: devices){
    //     //Serial.println((int)((BLEClient*)pair.second.peer_device)->getConnId());
    //     //Serial.println(((BLEClient*)pair.second.peer_device)->getRssi());
    //     Serial.printf("%i\n",((BLEClient*)pair.second.peer_device)->getRssi());
    // }
}