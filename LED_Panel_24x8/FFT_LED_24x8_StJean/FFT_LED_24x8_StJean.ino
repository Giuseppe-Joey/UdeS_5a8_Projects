/*
 File/Sketch Name: AudioFrequencyDetector

 Version No.: v1.0 Created 12 December, 2019
 
 Original Author: Clyde A. Lettsome, PhD, PE, MEM
 
 Description:  This code/sketch makes displays the approximate frequency of the loudest sound detected by a sound detection module. For this project, the analog output from the 
 sound module detector sends the analog audio signal detected to A0 of the Arduino Uno. The analog signal is sampled and quantized (digitized). A Fast Fourier Transform (FFT) is
 then performed on the digitized data. The FFT converts the digital data from the approximate discrete-time domain result. The maximum frequency of the approximate discrete-time
 domain result is then determined and displayed via the Arduino IDE Serial Monitor.

 Note: The arduinoFFT.h library needs to be added to the Arduino IDE before compiling and uploading this script/sketch to an Arduino.

 License: This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License (GPL) version 3, or any later
 version of your choice, as published by the Free Software Foundation.

 Notes: Copyright (c) 2019 by C. A. Lettsome Services, LLC
 For more information visit https://clydelettsome.com/blog/2019/12/18/my-weekend-project-audio-frequency-detector-using-an-arduino/

*/
#include <FastLED.h>

#define LED_PIN     6
#define MIC_PIN     A4

#define NUM_LEDS    192
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
#define UPDATES_PER_SECOND 100

#include "arduinoFFT.h"
#include "Arduino_AVRSTL.h"
#define SAMPLES 32                 //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 800   //Ts = Based on Nyquist, must be 2 times the highest expected frequency.

arduinoFFT FFT = arduinoFFT();
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

double dc[SAMPLES];

std::vector<int> indexArray;
std::vector<double> amplitudeArray;
std::vector<double> freqArray;

CRGB colorsArray[8] = {CRGB::Green, CRGB::Red, CRGB::Blue, CRGB::Orange, CRGB::Gold, CRGB::Purple, CRGB::Green, CRGB::Red};
CRGB stJeanArray[2] = {CRGB::Blue, CRGB::White};

bool firstTime = true;
int dc6 = 0;
void setup() 
{
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.setMaxPowerInVoltsAndMilliamps(5,500);
    

    currentBlending = LINEARBLEND;

    
    Serial.begin(115200); //Baud rate for the Serial Monitor
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}








void loop() 
{  
    /*Sample SAMPLES times*/
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
     
        vReal[i] = analogRead(MIC_PIN); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
        vImag[i] = 0; //Makes imaginary term 0 always

        /*remaining wait time between samples if necessary*/
        while(micros() < (microSeconds + samplingPeriod))
        {
          //do nothing
        }
    }
 
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);


int numLed[24];





if(firstTime){
  for(int count=0; count<SAMPLES;count++){
    dc[count]=vReal[count];
  }
  firstTime = false;
}




else
{
 int numLed[24];

for(int i=0; i < 24; i++){
 numLed[i] = (int)((vReal[i] - dc[i]) / 250);
}

for(int row = 0; row < 8; row++){
  for(int col = 0; col < 24; col++){
    if(row%2){
      if(numLed[col] < row+1)
        leds[row*24 + col] = CRGB::Black;
      else{
//        leds[row*24 + col] = CRGB::Blue; 
//        leds[row*24 + col] = colorsArray[col%8];
        leds[row*24 + col] = stJeanArray[col%2];
      }
    }

    else{
      if(numLed[col] < row+1)
        leds[row*24 + 23 - col] = CRGB::Black;
      else{
//        leds[row*24 + 23 - col] = CRGB::Blue;
//        leds[row*24 + 23 - col] = colorsArray[col%8];
        leds[row*24 + 23 - col] = stJeanArray[col%2];

      }
    }
  }
}
  FastLED.show();
}

  delay(50);
}
