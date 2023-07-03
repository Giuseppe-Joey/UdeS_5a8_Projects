/*
   Created on     :   Juin 26, 2023
   Last Update    :   Juin 29, 2023

   Program Name   :
   Program File   :   LED_glasses_remote_V0.ino
   Author         :   Giuseppe Lomonaco

   For            :   5@8 Génie: St-Jean-Baptiste
   Github         :   https://github.com/Giuseppe-Joey/project_Flag_St_Jean

   Program Description  :
   This project is based on the FASTLed Library for Arduino and it is used to 
   controll leds on a 32 leds glasses pair

*/


#include <FastLED.h>
#include <IRremote.h>

// code working with multiple of 8 for NUM_LEDS
const int LED_PIN = 3;
const int NUM_LEDS = 33;

uint8_t brightness = 50;            // nominal leds brightness
uint8_t fadeByBlack = 15;            // the amount of brightness to decrease
unsigned long n_milliseconds = 5;   // nominal leds switching speed
unsigned long lastCode;             //store the last IR code received from remote

const int RESET_PIN = 6;          // connected to reset pin and used to reset arduino
const int RECV_PIN = 2;           //IR remote controll receiver pin
const int RECV_LED_PIN = 5;       // led used to show if a good code is received from IR remote
IRrecv irrecv(RECV_PIN);
decode_results results;

CRGB leds[NUM_LEDS];    // creating the led array

DEFINE_GRADIENT_PALETTE( heatmap_gp )  {
    0,    0,    0,    0,        //black
  128,  255,    0,    0,        //red
  200,  255,  255,    0,        //bright yellow
  255,  255,  255,  255         //full white  
};

DEFINE_GRADIENT_PALETTE( stjeanOLD_gp )  {
      0,      0,      255,    245,        //Cyan
    46,   0,      21,    255,        //dark blue
    179,    255,   255,    255,        //white
      255,      0,      255,    245,        //Cyan
};

DEFINE_GRADIENT_PALETTE( red2blue_gp )  {
      0,      255,      0,    0,        //red
   46,   255,      0,    239,        //purple
    179,    0,   26,    255,        //blue
    255,  255,  255,  255,        //white
};

DEFINE_GRADIENT_PALETTE( green420_gp )  {
      0,    0,    255,    137,        //light green
      46,      0,      255,    0,        //green
   179,   255,      255,    0,        //yellow
    255,    255,   255,    255,        //white
};

DEFINE_GRADIENT_PALETTE( white_gp )  {
      0,    255,    255,    255,        //white
      46,      255,      255,    255,        //white
   179,   255,      255,    255,        //white
    255,    255,   255,    255,        //white
};

DEFINE_GRADIENT_PALETTE( red_gp )  {
      0,    255,    0,    0,        //red
      46,      255,      0,    0,        //red
   179,   255,      0,    0,        //red
    255,    255,   0,    0,        //red
};

DEFINE_GRADIENT_PALETTE( blue_gp )  {
      0,    0,    0,    255,        //blue
      46,      0,      0,    255,        //blue
   179,   0,      0,    255,        //blue
    255,    0,   0,    255,        //blue
};

DEFINE_GRADIENT_PALETTE( green_gp )  {
      0,    0,    255,    0,        //green
      46,      0,      255,    0,        //green
   179,   0,      255,    0,        //green
    255,    0,   255,    0,        //green
};

DEFINE_GRADIENT_PALETTE( purple_gp )  {
      0,    160,    32,    240,        //purple
      46,      160,    32,    240,    //purple
   179,   160,    32,    240,        //purple
    255,    160,    32,    240,        //purple
};

DEFINE_GRADIENT_PALETTE( pink_gp )  {
      0,    255,    192,    203,        //pink
      46,    255,    192,    203,    //pink
   179,   255,    192,    203,       //pink
    255,    255,    192,    203,        //pink
};


// creating palettes
CRGBPalette16 stJeanOLD_palette = stjeanOLD_gp;
CRGBPalette16 heat_palette = heatmap_gp;
CRGBPalette16 red2blue_palette = red2blue_gp;
CRGBPalette16 green420_palette = green420_gp;
CRGBPalette16 white_palette = white_gp;
CRGBPalette16 red_palette = red_gp;
CRGBPalette16 blue_palette = blue_gp;
CRGBPalette16 green_palette = green_gp;
CRGBPalette16 purple_palette = purple_gp;
CRGBPalette16 pink_palette = pink_gp;
CRGBPalette16 black_palette = black_gp;

CRGBPalette16 choice_palette = stJeanOLD_palette;
String choice_palette_string = "St-Jean OLD";



// ////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
    Serial.begin(9600);
    Serial.println("Initialization...");
    
    irrecv.enableIRIn();
    irrecv.blink13(true);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);  
    FastLED.setBrightness(brightness);
    FastLED.setCorrection(UncorrectedColor);
    FastLED.setTemperature(DirectSunlight);

    pinMode(RECV_LED_PIN, OUTPUT);   
    digitalWrite(RECV_LED_PIN, LOW);

    digitalWrite(RESET_PIN, HIGH);      // this line has to be before the pinMode() line to work
    pinMode(RESET_PIN, OUTPUT);   

    delay(3000);
}









// /////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

    digitalWrite(RECV_LED_PIN, LOW);
    digitalWrite(RESET_PIN, HIGH);


  // reading the IR receiver
  if (irrecv.decode(&results)){  
    if (results.value == 0xFFFFFFFF){
      results.value = lastCode;       // mandatory to detect wrong codes received
    }  
    
    lastCode = results.value;     // store received code from remote
    irrecv.resume();              // Receive the next value
    IRremote_switch_case();       // recognize button pressed on IRremote
  }
   

      
    EVERY_N_MILLISECONDS(n_milliseconds)  {
      // switch on an LED at random, choosing a random color from the palette
      leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(choice_palette, random8(), 255, LINEARBLEND);
    }
    
    // Fade all LEDS down by 1 in brightness each time this is called
    fadeToBlackBy(leds, NUM_LEDS, fadeByBlack);

    FastLED.setBrightness(brightness);
    FastLED.show(); 






    Serial.print("Run Time: ");
    Serial.print(millis() / 1000);
    Serial.print(" s;       ");
    
    Serial.print("IR: ");
    Serial.print(lastCode, HEX);   
    
    Serial.print(";        Brightness: ");  
    Serial.print(brightness); 
    Serial.print(" / 255");  
    
    Serial.print(";        n_milliseconds: ");  
    Serial.print(n_milliseconds); 

    Serial.print(";        fadeByBlack: ");  
    Serial.println(fadeByBlack); 
      
}



// declaring a reset function
void(* resetFunc) (void) = 0;




// /////////////////////////////////////////////////////////////////////////////////////////////////////
void IRremote_switch_case() {

   switch(lastCode){
  
      // ***** FIRST ROW *****
      // CH- button
      case 0xFFA25D:
        Serial.println("CH- button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        n_milliseconds = 1;
      break;
  
      // CH button
      case 0xFF629D:
        Serial.println("CH button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        n_milliseconds = 5;
      break;
  
      // CH+ button
      case 0xFFE21D:
        Serial.println("CH+ button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);        
        n_milliseconds = 10;
      break;

  
  
      // ***** SECOND ROW *****
      // PREV button
      case 0xFF22DD:
        Serial.println("PREV button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH); 
        if (fadeByBlack > 1)
          fadeByBlack = fadeByBlack - 1;
        else if (fadeByBlack == 1)
          fadeByBlack = 1;       
      break;
  
      // NEXT button
      case 0xFF02FD:
        Serial.println("NEXT button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        if (fadeByBlack < 50)
          fadeByBlack = fadeByBlack + 1;
        else if (fadeByBlack == 50)
          fadeByBlack = 50; 
      break;
  
      // PLAY/PAUSE button
      case 0xFFC23D:
        Serial.println("PLAY/PAUSE button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);  
        fadeByBlack = 15;               
      break;
  

  
      // ***** THIRD ROW *****
      // VOL- button
      case 0xFFE01F:
        Serial.println("VOL- button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        if (brightness > 20)
          brightness = brightness - 5;
        else if ((brightness <= 20) && (brightness >= 2))
          brightness = brightness - 1;
        else if (brightness == 1)
          brightness = 1; 
      break;
  
      // VOL+ button
      case 0xFFA857:
        Serial.println("VOL+ button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        if (brightness < 240)
          brightness = brightness + 5;
        else if ((brightness >= 240) && (brightness <= 253))
          brightness = brightness + 1;
        else if (brightness == 254)
          brightness = 254; 
      break;
  
      // EQ button
      case 0xFF906F:
        Serial.println("EQ button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        brightness = 50;
      break;



      // ***** FOURTH ROW *****
      // 0 button
      case 0xFF6897:
        Serial.println("0 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = stJeanOLD_palette;
        choice_palette_string = "St-Jean OLD";
      break;
  
      // 100+ button
      case 0xFF9867:
        Serial.println("100+ button pressed!"); 
        digitalWrite(RECV_LED_PIN, HIGH);   
        n_milliseconds = 25; 
      break;
  
      // 200+ button
      case 0xFFB04F:
        Serial.println("200+ button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        n_milliseconds = 50; 
      break;



      // ***** FIFTH ROW *****
      // 1 button
      case 0xFF30CF:
        Serial.println("1 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = heat_palette;
        choice_palette_string = "Heat";
      break;
  
      // 2 button
      case 0xFF18E7:
        Serial.println("2 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = red2blue_palette;
        choice_palette_string = "Red to Blue";
      break;
  
      // 3 button
      case 0xFF7A85:
        Serial.println("3 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = green420_palette;
        choice_palette_string = "Green 420";      
      break;



      // ***** SIXTH ROW *****
      // 4 button
      case 0xFF10EF:
        Serial.println("4 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = RainbowColors_p;
        choice_palette_string = "Preset Rainbow";
      break;
  
      // 5 button
      case 0xFF38C7:      
        Serial.println("5 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = red_palette;
        choice_palette_string = "Red";          
      break;
  
      // 6 button
      case 0xFF5AA5:
        Serial.println("6 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = blue_palette;
        choice_palette_string = "Blue"; 
      break;




      // ***** SEVENTH ROW *****
      // 7 button
      case 0xFF42BD:
        Serial.println("7 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = green_palette;
        choice_palette_string = "Green";         
      break;
  
      // 8 button
      case 0xFF4AB5:
        Serial.println("8 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        choice_palette = white_palette;
        choice_palette_string = "White";  
      break;
  
      // 9 button
      case 0xFF52AD:
        Serial.println("9 button pressed!");
        digitalWrite(RECV_LED_PIN, HIGH);
        delay(200);
        digitalWrite(RESET_PIN, LOW);
      break;
      }
}
