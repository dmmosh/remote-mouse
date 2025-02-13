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
const uint8_t mouse_report_desc[] = {
  USAGE_PAGE(1),       0x01, // USAGE_PAGE (Generic Desktop)
  USAGE(1),            0x02, // USAGE (Mouse)
  COLLECTION(1),       0x01, // COLLECTION (Application)
  USAGE(1),            0x01, //   USAGE (Pointer)
  COLLECTION(1),       0x00, //   COLLECTION (Physical)
  // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
  USAGE_PAGE(1),       0x09, //     USAGE_PAGE (Button)
  USAGE_MINIMUM(1),    0x01, //     USAGE_MINIMUM (Button 1)
  USAGE_MAXIMUM(1),    0x05, //     USAGE_MAXIMUM (Button 5)
  LOGICAL_MINIMUM(1),  0x00, //     LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1),  0x01, //     LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),      0x01, //     REPORT_SIZE (1)
  REPORT_COUNT(1),     0x05, //     REPORT_COUNT (5)
  HIDINPUT(1),         0x02, //     INPUT (Data, Variable, Absolute) ;5 button bits
  // ------------------------------------------------- Padding
  REPORT_SIZE(1),      0x03, //     REPORT_SIZE (3)
  REPORT_COUNT(1),     0x01, //     REPORT_COUNT (1)
  HIDINPUT(1),         0x03, //     INPUT (Constant, Variable, Absolute) ;3 bit padding
  // ------------------------------------------------- X/Y position, Wheel
  USAGE_PAGE(1),       0x01, //     USAGE_PAGE (Generic Desktop)
  USAGE(1),            0x30, //     USAGE (X)
  USAGE(1),            0x31, //     USAGE (Y)
  USAGE(1),            0x38, //     USAGE (Wheel)
  LOGICAL_MINIMUM(1),  0x81, //     LOGICAL_MINIMUM (-127)
  LOGICAL_MAXIMUM(1),  0x7f, //     LOGICAL_MAXIMUM (127)
  REPORT_SIZE(1),      0x08, //     REPORT_SIZE (8)
  REPORT_COUNT(1),     0x03, //     REPORT_COUNT (3)
  HIDINPUT(1),         0x06, //     INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
  // ------------------------------------------------- Horizontal wheel
  USAGE_PAGE(1),       0x0c, //     USAGE PAGE (Consumer Devices)
  USAGE(2),      0x38, 0x02, //     USAGE (AC Pan)
  LOGICAL_MINIMUM(1),  0x81, //     LOGICAL_MINIMUM (-127)
  LOGICAL_MAXIMUM(1),  0x7f, //     LOGICAL_MAXIMUM (127)
  REPORT_SIZE(1),      0x08, //     REPORT_SIZE (8)
  REPORT_COUNT(1),     0x01, //     REPORT_COUNT (1)
  HIDINPUT(1),         0x06, //     INPUT (Data, Var, Rel)
  END_COLLECTION(0),         //   END_COLLECTION
  END_COLLECTION(0)          // END_COLLECTION
};



void setup(){
    setCpuFrequencyMhz(240);
    //Serial.begin(115200);
    //Serial.println("Starting BLE work!");
    pinMode(LED,OUTPUT);
    pinMode(JOYSTICK,OUTPUT);
    digitalWrite(JOYSTICK,OFF);
    digitalWrite(LED,OFF);

    
    //analogSetAttenuation(ADC_11db);

    BLEDevice::init("Remote Mouse");
    //BLEDevice::setCustomGattsHandler(my_gatts_event_handler);

    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9); 
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN ,ESP_PWR_LVL_P9);
    

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyCallbacks());
    //pServer->getPeerDevices(false);

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

   
    
    hid->setBatteryLevel(100);
    digitalWrite(LED,ON);
  
    pinMode(SW, INPUT_PULLUP);
    loop();
}

int x = 0;
int y = 0;
uint8_t click = 0;
void loop() {

    if(!connected){
      digitalWrite(JOYSTICK, OFF);
      pAdvertising->start();
      connect_wait();
      digitalWrite(JOYSTICK, ON);
      digitalWrite(LED,ON);
    }  
    
    
    x = (21-analogRead(VRX)/128)>>2; // 2750 / 64 = 42
    y = (21-analogRead(VRY)/128)>>2; // 2730 / 64 = 42

    click = (bool)!digitalRead(SW);
    
    x+= (x>0) ? -(x>>1) : 1;
    y += (y>0) ? -(y>>1) : 1;
    
    if(abs(x)>10) x/=2;
    if(abs(y)>10) y/=2;


    //+Serial.printf("X: %i Y: %i click: %i\n", (int8_t)x,(int8_t)y,click);

    if(y){
      move(click,x,y);
      delay(8);
    } else if(x){
      move(click,x);
      delay(8);
    } else if(click){
      move(click);
      delay(8);
    } else {
      delay(1);
    }

    

}