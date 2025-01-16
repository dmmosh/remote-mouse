#include "header.h"



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