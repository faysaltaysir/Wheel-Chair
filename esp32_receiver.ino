#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;
int dirc,spd;


#define RXp2 16
#define TXp2 17

#define pin 13

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name // <------- set this to be the same as the name you chose above!!!!!
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
  pinMode(pin,INPUT_PULLUP);

}

uint8_t calculate_checksum(uint8_t *data) {
  uint8_t checksum = 0;
  checksum |= 0b11000000 & data[1];
  checksum |= 0b00110000 & data[2];
  checksum |= 0b00001100 & data[3];
  checksum |= 0b00000011 & data[4];
  return checksum;
}

void loop() {
  int state = digitalRead(pin);
  if(state == 0){hand_control();}
  
  else{voice_control();}
  
}


void hand_control(){
    
  uint8_t recv_data[6];
  if (SerialBT.available()) {
   
    SerialBT.readBytes(recv_data, 6);
    
    if (recv_data[0] != 'T') {
      Serial.print("Receive error!");
      return;
    }

    if (recv_data[5] != calculate_checksum(recv_data)) {
      Serial.print("Decode error!");
      return;
    }
     dirc = recv_data[1];
  
     spd = recv_data[2];
     Serial.print("hand mood activated      ");
     Serial.print(dirc);
     Serial.write('\n');
     delay(100);
}
}

void voice_control(){
    Serial.print("voice command:           ");
    Serial.println(Serial2.readString());
}
