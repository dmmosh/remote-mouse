#include "header.h"


void move(uint8_t x , uint8_t y, uint8_t xwheel, uint8_t ywheel){
    uint8_t packet[] = {0,x,y,xwheel,ywheel};
    input->setValue(packet, 5);
    input->notify();
}

void my_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gatts_cb_param_t* param){
    switch (event) {
        case ESP_GATTS_CONGEST_EVT:
            if (param->congest.congested) {
                Serial.println("CONGESTED");
            } else {
                Serial.println("CLEARED");
            }
            break;

        // Handle other BLE GATT server events as needed
        default:
            break;
    }
}


 void connect_wait(){
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