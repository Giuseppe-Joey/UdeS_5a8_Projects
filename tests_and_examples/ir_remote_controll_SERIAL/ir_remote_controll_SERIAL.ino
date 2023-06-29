/*
   Created on     :   Mai 4, 2023
   Last Update    :   Mai 10, 2023

   Program Name   :
   Program File   :   Projet_Flag_St_Jean.ino
   Author         :   Giuseppe Lomonaco

   For            :   5@8 Génie: St-Jean-Baptiste
   Github         :   https://github.com/Giuseppe-Joey/project_Flag_St_Jean

   Program Description  :
   This project is based on the FASTLed Library for Arduino and it is used to 
   controll leds on a 


   HEX CODES:
   
   CH-  : FFA25D
   CH   : FF629D  
   CH+  : FFE21D
   PREV : FF22DD
   NEXT : FF02FD
   PLAY : FFC23D
   VOL- : FFE01F
   VOL+ : FFA857
   EQ   : FF906F
   0    : FF6897
   100+ : FF9867
   200+ : FFB04F
   1    : FF30CF
   2    : FF18E7
   3    : FF7A85
   4    : FF10EF
   5    : FF38C7
   6    : FF5AA5
   7    : FF42BD
   8    : FF4AB5
   9    : FF52AD
       
   
*/



#include <FastLED.h>
#include <Wire.h> 
#include <IRremote.h>



const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

// define variable to store the last code received
unsigned long lastCode;
uint8_t brightness = 0;







// ////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

    Serial.begin(9600);
    irrecv.enableIRIn();
    irrecv.blink13(true);


    Serial.println("Initialization...");
    delay(1000);
}







// /////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  
  EVERY_N_MILLISECONDS(1000)   {
    Serial.print("Run Time: ");
    Serial.print(millis() / 1000);
    Serial.print(" s;       ");
    Serial.print("IR: ");
    Serial.print(lastCode, HEX);   
    Serial.print(";        brightness: ");  
    Serial.println(brightness); 
  }

  

  // reading the IR receiver
  if (irrecv.decode(&results)){  
    if (results.value == 0xFFFFFFFF){
      results.value = lastCode;
    }  

    // store received code from remote
    lastCode = results.value;
    


    switch(lastCode){
  
      // ***** FIRST ROW *****
      // CH- button
      case 0xFFA25D:
      Serial.println("CH- button pressed!");
      break;
  
      // CH button
      case 0xFF629D:
      Serial.println("CH button pressed!");
      break;
  
      // CH+ button
      case 0xFFE21D:
      Serial.println("CH+ button pressed!");
      break;
  
  
      // ***** SECOND ROW *****
      // PREV button
      case 0xFF22DD:
      Serial.println("PREV button pressed!");
      break;
  
      // NEXT button
      case 0xFF02FD:
      Serial.println("NEXT button pressed!");
      break;
  
      // PLAY/PAUSE button
      case 0xFFC23D:
      Serial.println("PLAY/PAUSE button pressed!");
      break;
  
  
      // ***** THIRD ROW *****
      // VOL- button
      case 0xFFE01F:
      Serial.println("VOL- button pressed!");
      brightness = brightness - 1;
      break;
  
      // VOL+ button
      case 0xFFA857:
      Serial.println("VOL+ button pressed!");
      brightness = brightness + 1;
      break;
  
      // EQ button
      case 0xFF906F:
      Serial.println("EQ button pressed!");
      break;

    }
    
      // resume reading IR sensor ???
      irrecv.resume();
    

  }

  // delay in between reads for stability
  delay(100);
}
