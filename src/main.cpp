#include "header.h"
// variables for the bluetooth server and hid device
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLEAdvertising *pAdvertising;
BLEServer *pServer;
BLEScan* pBLEScan;
bool connected =false;


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



void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    pinMode(LED,OUTPUT);
    digitalWrite(LED,ON);

    BLEDevice::init("Remote Mouse");

    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); 
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);
    BLEDevice::setCustomGattsHandler(my_gatts_event_handler);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyCallbacks());
    pServer->getPeerDevices(false);

    hid = new BLEHIDDevice(pServer);
    input = hid->inputReport(0); // <-- input REPORTID from report map
    //output = hid->outputReport(1); // <-- output REPORTID from report map


    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x02);

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    hid->reportMap((uint8_t*)mouse_report_desc,sizeof(mouse_report_desc));

    hid->startServices();

    pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();

    pBLEScan = BLEDevice::getScan();  
    pBLEScan->setActiveScan(true);  // Set active scan to get more information (e.g., RSSI)
    pBLEScan->setInterval(100);     // Set scan interval (in milliseconds)
    pBLEScan->setWindow(99);        // Set scan window (in milliseconds)
    
    hid->setBatteryLevel(100);
  


    //ESP_LOGD(LOG_TAG, "Advertising started!");
    //delay(portMAX_DELAY);
}


void loop() {

    if(!connected){
      pAdvertising->start();
      connect_wait();
    }  
    move(10,10);
    vTaskDelay(20/portTICK_PERIOD_MS);
}