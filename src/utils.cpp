#include "header.h"


void move(uint8_t click,uint8_t x , uint8_t y){
    uint8_t packet[] = {click,x,y};
    if(connected)
        input->setValue(packet, sizeof(packet));
    if(connected)
        input->notify();
}

void move(uint8_t click,uint8_t x ){
    uint8_t packet[] = {click,x};
    if(connected)
        input->setValue(packet, sizeof(packet));
    if(connected)
        input->notify();
};

void move(uint8_t click){
    if(connected)
        input->setValue(&click, 1);
    if(connected)
        input->notify();
};



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

void my_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gatts_cb_param_t* param){
    switch (event) {
               case 0:       Serial.println("EVENT #0:   register application id");
        break; case 1:       Serial.println("EVENT #1:   gatt client request read operation");
        break; case 2:       Serial.println("EVENT #2:   gatt client request write operation");
        break; case 3:       Serial.println("EVENT #3:   gatt client request execute write");
        break; case 4:       Serial.println("EVENT #4:   set mtu complete");
        break; case 5:       Serial.println("EVENT #5:   receive confirm");
        break; case 6:       Serial.println("EVENT #6:   unregister application id");
        break; case 7:       Serial.println("EVENT #7:   create service complete");
        break; case 8:       Serial.println("EVENT #8:   add included service complete");
        break; case 9:       Serial.println("EVENT #9:   add characteristic complete");
        break; case 10:      Serial.println("EVENT #10:  add descriptor complete");
        break; case 11:      Serial.println("EVENT #11:  delete service complete");
        break; case 12:      Serial.println("EVENT #12:  start service complete");
        break; case 13:      Serial.println("EVENT #13:  stop service complete");
        break; case 14:      Serial.println("EVENT #14:  gatt client connect");
        break; case 15:      Serial.println("EVENT #15:  gatt client disconnect");
        break; case 16:      Serial.println("EVENT #16:  connect to peer");
        break; case 17:      Serial.println("EVENT #17:  disconnect from peer");
        break; case 18:      Serial.println("EVENT #18:  gatt server close");
        break; case 19:      Serial.println("EVENT #19:  gatt listen to be connected the event comes");
        break; case 20:      Serial.println("EVENT #20:  congest happen");
        break; case 21:      Serial.println("EVENT #21:  gatt send response complete");
        break; case 22:      Serial.println("EVENT #22:  gatt create table complete");
        break; case 23:      Serial.println("EVENT #23:  gatt set attr value complete");
        break; case 24:      Serial.println("EVENT #24:  gatt send service change indication complete");

        // Handle other BLE GATT server events as needed
        default:
            Serial.printf("EVENT #%i: unknown", event);
    }
}