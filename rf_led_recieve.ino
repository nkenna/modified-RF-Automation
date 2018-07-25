#include <VirtualWire.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);



#define DHTPIN 7     // what digital pin we're connected to
const int RF_RX_PIN = 11;


int relayFanPin = 10; //attach relay to pin D4
int ledPin = 5; //attach relay to pin D5
int errorLED = 13;
int pirPin = 2;
int relayLampPin = 3;
int relayLockPin = 4;

float count3 = 120;
 

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

boolean led = 1;
boolean relayFan = 1;
boolean relayLamp = 1;
boolean relayLock = 1;
boolean ledState = true;

uint8_t buf [VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;

float lock_close = millis();
boolean lock_flag = false;

void setup() {
  led = 1;
relayFan = 1;
relayLamp = 1;
relayLock = 1;
ledState = true;
  // put your setup code here, to run once:
  dht.begin();
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(RF_RX_PIN);
  vw_setup(2000);
  vw_rx_start();

  pinMode(relayFanPin, OUTPUT);
   pinMode(relayLampPin, OUTPUT);
    pinMode(relayLockPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(errorLED, OUTPUT);



  digitalWrite(relayFanPin, LOW);
  digitalWrite(relayLampPin, LOW);
  digitalWrite(relayLockPin, LOW);
  digitalWrite(ledPin, LOW);

 // relay = false;
  //led = false;
  Serial.begin(9600);

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

   // display a line of text
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("RF Automation");
  

  // update display with all of the above graphics
  display.display();

   //attach interrupt for the various pins
  attachInterrupt(digitalPinToInterrupt(pirPin), Alert_PIR, RISING);
  
}

void Alert_PIR(){
  Serial.println("Motion Detected in PIR1");
  Serial.println("Sending SMS Alert");
  display.setCursor(0,40);
  display.print("Motion Detected");
}

void loop() {

  display.setCursor(0,0);
  display.print("RF Automation");
  
  // put your main code here, to run repeatedly:
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

 display.setCursor(0,10);
  display.print("Temp: ");
  display.print(t);
  display.print(" C");
  
  display.setCursor(0,20);
  display.print("Humidity: ");
  display.print(h);
  display.print("%");
  display.display();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  
  
  
  
  if(vw_get_message(buf, &buflen))  { 
    delay(500); 
    Serial.println((char *) buf);
    payloadDecode ( (char *) buf); 
    delay(500);   
    } 

    if(led == true)  { 
      
         digitalWrite(ledPin, led);   
      }else{
        ledState = !ledState;
        Serial.println(led);
        digitalWrite(ledPin, led);
        
        display.setCursor(0,30);
  display.print("LED ON");
 
  display.display();
    display.clearDisplay();  
        
        
     //   display.setCursor(0,30);
 // display.print("LED OFF");
 
//  display.display();
 // display.clearDisplay();
      }


      if(relayFan == true){
         
        Serial.println(relayFan);
        digitalWrite(relayFanPin, relayFan);
  //display.clearDisplay();
      }else{
        Serial.println(relayFan);
        digitalWrite(relayFanPin, relayFan);

        display.setCursor(0,30);
  display.print("FAN ON");
 
  display.display();
  
       // display.setCursor(0,30);
  //display.print("FAN OFF");
 
 // display.display();
      }

      if(relayLamp == true){
        Serial.println(relayLamp);
        digitalWrite(relayLampPin, relayLamp);
  //display.clearDisplay();
      }else{
        Serial.println(relayLamp);
        digitalWrite(relayLampPin, relayLamp);

        display.setCursor(0,30);
  display.print("AC LAMP ON");
 
  display.display();
        
   //     display.setCursor(0,30);
  //display.print("AC LAMP OFF");
 
  //display.display();
  //display.clearDisplay();
      }

 if(relayLock == true){
  count3 = 120;
        Serial.println(relayLock);
        digitalWrite(relayLockPin, relayLock);

        //display.setCursor(0,30);
 // display.print("DOOR LOCKED");
 
 // display.display();
//display.clearDisplay();
      }else{
        Serial.println(relayLock);
        digitalWrite(relayLockPin, relayLock);
        display.setCursor(0,30);
 display.print("DOOR UNLOCKED");
 
 display.display();
  //display.clearDisplay();
  Serial.println(lock_close);
  
  //close lock after some minutes
  Serial.println(millis() - lock_close);
  if (millis() - lock_close > 120000){
   // Serial.println(millis() - lock_close);
    lock_flag = true;
    lock_close = millis();
  }

  count3 -= 1;
  display.setCursor(0,45);
  display.print("DOOR LOCKING IN ");
 display.print(count3);
 display.print("secs" );
  display.display();
      }
//  
//if(relayFan == true)  { 
//  digitalWrite(relayFanPin, HIGH);
//  }
//  
//if(relayFan == false)  { 
//  digitalWrite(relayFanPin, LOW);
//  }



if (lock_flag){
int  count1 = 0;
  int count2 = 5;
  
  while(count1 < 5){
     display.setCursor(0,30);
    display.print("DOOR LOCKING IN ");
    display.print(count2 -= 1);
     display.display();
    display.clearDisplay();
     count1 ++;
     delay(1000);
  }
  relayLock = 1;
  digitalWrite(relayLockPin, relayLock);
  lock_flag = false;
  
     
}

display.clearDisplay();
  delay(400);
}


void payloadDecode(char msg[]) { 
  if (strlen(msg) == 0)   { 
    
    ErrorLED(); 
    return;
    }
      
if ( compareMessage(msg, '@') == false || compareMessage(msg, '$') == false)  { 
  //Serial.println(msg);
  ErrorLED(); 
  return;
  }
    
char cTag[5] =""; 
int index =0; 
int iLoop = IndexOf(msg,'@');

if(iLoop == -1) { 
  //Serial.println(msg);
  ErrorLED(); 
  return; 
  }
  
 iLoop++;

while (iLoop < strlen(msg)) {
  
  if(msg[iLoop] == '$')   { 
    break;   
    } else   {
      cTag[index] = msg[iLoop]; 
    //  Serial.println(msg);
      index++;

 }
   iLoop ++; 
   }
   
 String sTag = String(cTag);

if (sTag.equals("ABC"))   {
  //Serial.println(msg);
  led = !led;   
  }
 
  
if (sTag.equals("FON"))   { 
  //Serial.println(msg);    
  relayFan = !relayFan;   
  }

  if (sTag.equals("LCK"))   { 
  //Serial.println(msg);    
  relayLock = !relayLock;   
  }

  if (sTag.equals("LON"))   { 
  //Serial.println(msg);    
  relayLamp = !relayLamp;   
  }
  
   
}

int IndexOf(char msg[], char tag) { 
  if (strlen(msg) ==0) {
    return false;
  }
    
    boolean bFlag = false; 
    int iIndex = -1;
    
for( int i=0; i<strlen(msg); i++)  { 
  if(msg[i]== tag)    
    iIndex = i;
    }

return iIndex;
}


boolean compareMessage (char msg[], char tag) { 
  if (strlen(msg) ==0){
    return false;  
  }
  
  boolean bFlag = false;

//Serial.println(msg);

for( int i=0; i<strlen(msg); i++)  { 
  if(msg[i]== tag)    
  bFlag = true;  
  }
  
return bFlag;
}



void ErrorLED() {
  digitalWrite(errorLED, HIGH); 
  delay(500); 
  digitalWrite(errorLED, LOW);
}

