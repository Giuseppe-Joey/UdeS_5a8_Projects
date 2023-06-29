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
   
   CH-  : 0FA25D81
   CH   : 0F629D81  
   CH+  : 0FE21D81
   PREV : 0F22DD81
   NEXT : 0F02FDD4
   PLAY : 0FC23D52
   VOL- : 0FE01F52
   VOL+ : 0FA85752
   EQ   : 0F
   0    : 0F
   100+ : 0F
   200+ : 0F
   1    : 0F
   2    : 0F
   3    : 0F
   4    : 0F
   5    : 0F
   6    : 0F
   7    : 0F
   8    : 0F
   9    : 0F
       
   
*/




#include <Wire.h> 
#include <IRremote.h>
#include <LiquidCrystal.h>
 
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

// define variable to store the last code received
unsigned long lastCode;









// ////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

    irrecv.enableIRIn();
    irrecv.blink13(true);


    lcd.begin(16, 2); //initialize the lcd
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Init...");

    delay(1000);
}







// /////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.setCursor(6, 0);           // move cursor to second line "1" and 9 spaces over
    lcd.print(millis() / 1000);    // display seconds elapsed since power-up
    lcd.setCursor(0, 1);
    lcd.print("IR: ");
   // lcd.setCursor(4, 1);
   // lcd.print(lastCode, HEX);

  
  if (irrecv.decode(&results)){

    //lastCode = results.value;
    
    if (results.value == 0xFFFFFFFF){
      results.value = lastCode;
    }
    
    lcd.setCursor(4, 1);
    lcd.print(results.value, HEX);
    irrecv.resume();
  }

  delay(30);
}
