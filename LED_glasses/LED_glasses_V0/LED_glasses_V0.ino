/*
   Created on     :   Juin 26, 2023
   Last Update    :   Juin 29, 2023

   Program Name   :
   Program File   :   LED_glasses_V0.ino
   Author         :   Giuseppe Lomonaco

   For            :   5@8 Génie: St-Jean-Baptiste
   Github         :   https://github.com/Giuseppe-Joey/project_Flag_St_Jean

   Program Description  :
   This project is based on the FASTLed Library for Arduino and it is used to 
   controll leds on a 32 leds glasses pair

*/


#include <FastLED.h>

#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define DATA_PIN        3
#define NUM_LEDS        33

unsigned long n_milliseconds = 25;
uint8_t brightness = 50;

// creating the led array
CRGB leds[NUM_LEDS];


DEFINE_GRADIENT_PALETTE( heatmap_gp )  {
    0,    0,    0,    0,        //black
  128,  255,    0,    0,        //red
  200,  255,  255,    0,        //bright yellow
  255,  255,  255,  255,         //full white  
};

DEFINE_GRADIENT_PALETTE( heatmapV2_gp )  {
    0,  255,    255,    0,        //bright yellow
  128,  255,    165,    0,          //orange
  200,  255,      0,    0,              //red
  255,  255,    255,  255,             //full white  
};

DEFINE_GRADIENT_PALETTE( stjean_gp )  {
    0,    0,  255,    245,        //Cyan
   46,    0,   21,    255,        //dark blue
  179,  255,  255,    255,        //white
  255,    0,  255,    245,        //Cyan
};

// creating palettes
CRGBPalette16 stJean_palette = stjean_gp;
//CRGBPalette16 heat_palette = heatmap_gp;
//CRGBPalette16 heatV2_palette = heatmapV2_gp;



// ////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
    Serial.begin(57600);
    
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 25);
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  
    FastLED.setBrightness(brightness);
    FastLED.setCorrection(UncorrectedColor);
    FastLED.setTemperature(DirectSunlight);
    
    delay(1000);
}




  
// /////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {


    EVERY_N_MILLISECONDS(n_milliseconds)  {
      // switch on an LED at random, choosing a random color from the palette
      leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(stJean_palette, random8(), 255, LINEARBLEND);
    }

    // Fade all LEDS down by 1 in brightness each time this is called
    fadeToBlackBy(leds, NUM_LEDS, 1);


    FastLED.show(); 
}
