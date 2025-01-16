#include "header.h"
// variables for the bluetooth server and hid device
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLEAdvertising *pAdvertising;
BLEServer *pServer;
BLEScan* pBLEScan;
bool connected = false;


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

    hid->manufacturer()->setValue("Remote Mouse");

    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x02);

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    hid->startServices();
    pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
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
      pAdvertising->start();
      connect_wait();
    }  
      digitalWrite(LED,ON);
    vTaskDelay(5/portTICK_PERIOD_MS);
}