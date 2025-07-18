#include "header.h"

// variables for the bluetooth server and hid device
bool connected;
int16_t x,y;
uint8_t click;
BluetoothSerial SerialBT;


/*
git-all && pio run --target upload --target monitor
*/



void setup(){
    setCpuFrequencyMhz(240);
    Serial.begin(115200);
    pinMode(LED,OUTPUT);
    pinMode(JOYSTICK,OUTPUT);
    digitalWrite(JOYSTICK,HIGH);
    digitalWrite(LED,OFF);
    SerialBT.begin("Remote Mouse");


    connected = false;
    x = 0; y= 0; click = 0;

  
    digitalWrite(LED,ON);
    pinMode(SW, INPUT_PULLUP);
    loop();
}

void loop() {

    if(!SerialBT.hasClient()){
      uint16_t ms = 0;
      bool on = true;
      while(!SerialBT.hasClient()){
        if(!(ms%1000)){
          on = ~on;
          digitalWrite(LED,on);
        }
        ms++;
        delay(1);
      }
      digitalWrite(LED,ON);
    }

    // if(!connected){
    //   digitalWrite(JOYSTICK, OFF);
    //   pAdvertising->start();
    //   connect_wait();
    //   digitalWrite(JOYSTICK, ON);
    //   digitalWrite(LED,ON);
    // }  
    
    
    // x = (21-analogRead(VRX)/128); // 2750 / 64 = 42
    // y = (21-analogRead(VRY)/128); // 2730 / 64 = 4
     click = (bool)!digitalRead(SW);

    // x+= (x>0) ? -(x>>1) : 1;
    // y += (y>0) ? -(y>>1) : 1;

    // 3325 max on both

    if(SerialBT.available()){
      x = analogRead(VRX) >>8;
      y= analogRead(VRY)>>8;
      Serial.printf("%i %i %i\n",click,x,y);

    }
    delay(1);

    

}