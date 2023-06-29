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
#define NUM_LEDS    128
#define BRIGHTNESS  20
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
#define UPDATES_PER_SECOND 100

#include "arduinoFFT.h"
#include "Arduino_AVRSTL.h"
#define SAMPLES 16             //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.

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


bool firstTime = true;
int dc6 = 0;
void setup() 
{
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    //FastLED.setBrightness(  BRIGHTNESS );
    FastLED.setMaxPowerInVoltsAndMilliamps(5,200);
    

    currentBlending = LINEARBLEND;

    
    Serial.begin(115200); //Baud rate for the Serial Monitor
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
}
void AnalyseSound(double *vD, uint16_t samples, double samplingFrequency)
{
  for (uint16_t i = 1; i < ((samples >> 1) + 1); i++) {
    
    indexArray.push_back(i);
    amplitudeArray.push_back(vD[i]);
    
  }
  int IndexOfMaxY = 0;
  for(int z =0; z<indexArray.size(); z++)
  {
    IndexOfMaxY = indexArray[z];
    double delta = 0.5 * ((vD[IndexOfMaxY-1] - vD[IndexOfMaxY+1]) / (vD[IndexOfMaxY-1] - (2.0 * vD[IndexOfMaxY]) + vD[IndexOfMaxY+1]));
    double interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples-1);
    if(IndexOfMaxY==(samples >> 1)) //To improve calculation on edge values
      interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples);
    freqArray.push_back(interpolatedX);
  }
}
void CustomMajorPeak(double *vD, uint16_t samples, double samplingFrequency)
{

  double maxY = 0;
  //If sampling_frequency = 2 * max_frequency in signal,
  //value would be stored at position samples/2
  for (uint16_t i = 1; i < ((samples >> 1) + 1); i++) {
    if ((vD[i-1] < vD[i]) && (vD[i] > vD[i+1])) {
      indexArray.push_back(i);
      amplitudeArray.push_back(vD[i]);
    }
  }
  int IndexOfMaxY = 0;
  for(int z =1; z<indexArray.size(); z++)
  {
    IndexOfMaxY = indexArray[z];
    double delta = 0.5 * ((vD[IndexOfMaxY-1] - vD[IndexOfMaxY+1]) / (vD[IndexOfMaxY-1] - (2.0 * vD[IndexOfMaxY]) + vD[IndexOfMaxY+1]));
    double interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples-1);
    if(IndexOfMaxY==(samples >> 1)) //To improve calculation on edge values
      interpolatedX = ((IndexOfMaxY + delta)  * samplingFrequency) / (samples);
    freqArray.push_back(interpolatedX);
  }
}
void loop() 
{  
    /*Sample SAMPLES times*/
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
     
        vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
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

    /*Find peak frequency and print peak*/
    //AnalyseSound(vReal, SAMPLES, SAMPLING_FREQUENCY);
    
//    for(int count=0;count<freqArray.size(); count++)
//    {
//      Serial.print(freqArray[count]);
//      Serial.print(" : ");
//      Serial.print(amplitudeArray[count]);
//      Serial.println();
//    }

//Serial.print(freqArray[3]);
//Serial.print(" : ");
if(firstTime)
{
  for(int count=0; count<SAMPLES;count++){
    dc[count]=vReal[count];
  }

  firstTime = false;
  
}
else
{
  for( int i =0; i<15; i++){
    int numLed= (int) (vReal[i]-dc[i]);
    if(numLed>8)
      numLed= 8;
    else if(numLed<0)
      numLed=0;
    
    if( i%2 ==0){
      for( int y= i*8; y<i*8+8; y++){
        if(y<i*8+numLed)
        {
          leds[y] = colorsArray[i%8];
        }
        else{
          leds[y] = CRGB::Black;
        }
      }
    }
    else{
      for( int z= (i+1)*8; z> (i+1)*8-8; z--){
        if(z> (i+1)*8-numLed)
        {
          leds[z] = colorsArray[i%8];
        }
        else{
          leds[z] = CRGB::Black;
        }
      }
    } 

  }
    FastLED.show();
}

/*
Serial.print(freqArray[6]);
Serial.print(" : ");
Serial.print(amplitudeArray[6]);
Serial.println();

Serial.print(freqArray[10]);
Serial.print(" : ");
Serial.print(amplitudeArray[10]);
Serial.println();

    indexArray.clear();
    amplitudeArray.clear();
    freqArray.clear();*/
 
    /*Script stops here. Hardware reset required.*/
    //while (1); //do one time
    delay(50);
}
