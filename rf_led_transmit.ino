#include <VirtualWire.h>

const int RF_TX_PIN = 8;
const int pinLamp_ON_OFF = 2;
const int pinLock_ON_OFF = 3;
const int pinFan_ON_OFF = 5;
const int pinLED_ON_OFF = 6;
int pinLed = 4;

char payload[6] = "";

int relayLamp_on_off_Value = 0;
int relayFan_on_off_Value = 0;
int relayLock_on_off_Value = 0;
int relayLed_on_off_Value = 0;


void setup() {
  // put your setup code here, to run once:
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);

  pinMode(pinLED_ON_OFF, INPUT_PULLUP);
  pinMode(pinLamp_ON_OFF, INPUT_PULLUP);
  pinMode(pinLock_ON_OFF, INPUT_PULLUP);
  pinMode(pinFan_ON_OFF, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);

 
}

void loop() {
  // put your main code here, to run repeatedly:
  relayLamp_on_off_Value = digitalRead(pinLamp_ON_OFF);
  relayLock_on_off_Value = digitalRead(pinLock_ON_OFF);
  relayFan_on_off_Value = digitalRead(pinFan_ON_OFF);
  relayLed_on_off_Value = digitalRead(pinLED_ON_OFF);

  //using pullup, the values are inverted
  if(relayLed_on_off_Value == LOW){
    digitalWrite(pinLed, HIGH);
    strcpy(payload, "@ABC$");

    if(strlen(payload) > 0){
      sendPayload(payload);
      }
  delay(500);
    digitalWrite(pinLed, LOW);
  }

 

   if(relayFan_on_off_Value == LOW ){
    digitalWrite(pinLed, HIGH);
    strcpy(payload, "@FON$");
    
    if(strlen(payload) > 0){
      sendPayload(payload);
      }
   digitalWrite(pinLed, LOW);
    
  }

    if(relayLamp_on_off_Value == LOW ){
    digitalWrite(pinLed, HIGH);
    strcpy(payload, "@LON$");
    
    if(strlen(payload) > 0){
      sendPayload(payload);
      }
   digitalWrite(pinLed, LOW);
    
  }

    if(relayLock_on_off_Value == LOW ){
    digitalWrite(pinLed, HIGH);
    strcpy(payload, "@LCK$");
    
    if(strlen(payload) > 0){
      sendPayload(payload);
      }
   digitalWrite(pinLed, LOW);
    
  }
  

 strcpy(payload, "");
 resetLoads();

}

void sendPayload(char payload[]){
  vw_send((uint8_t *)payload, strlen(payload));
  Serial.println(payload);
  vw_wait_tx();
  delay(400);
}

void resetLoads(){
  relayLamp_on_off_Value = 0;
  relayFan_on_off_Value = 0;
  relayLock_on_off_Value = 0;
  relayLed_on_off_Value = 0;
}

