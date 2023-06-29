



#include <FastLED.h>

#define LED_PIN     6
#define MIC_PIN     A4
#define NUM_LEDS    100
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
#define SAMPLING_FREQUENCY 8000 //Ts = Based on Nyquist, must be 2 times the highest expected frequency.

arduinoFFT FFT = arduinoFFT();
 
unsigned int samplingPeriod;
unsigned long microSeconds;
 
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

//double dc[SAMPLES];
std::vector<double> dc;


std::vector<int> indexArray;
std::vector<double> amplitudeArray;
std::vector<double> freqArray;

CRGB colorsArray[8] = {CRGB::Green, CRGB::Red, CRGB::Blue, CRGB::Orange, CRGB::Gold, CRGB::Purple, CRGB::Green, CRGB::Red};


bool firstTime = true;



void setup() 
{
    Serial.begin(115200); //Baud rate for the Serial Monitor
    
    delay( 3000 ); // power-up safety delay
    
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    
    //FastLED.setBrightness(  BRIGHTNESS );
    FastLED.setMaxPowerInVoltsAndMilliamps(5,200);
    
    currentBlending = LINEARBLEND;
    
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


if(firstTime) {
  for(int count = 0; count < SAMPLES; count++) {
    dc.push_back(vReal[count]);
  }

  firstTime = false;
}



else  {
  for( int i = 0; i < 10; i++) {
    int numLed = ((int)(vReal[i]-dc[i])) / 200;
    
    if(numLed>10)
      numLed= 10;
    else if(numLed<0)
      numLed=0;
    
    Serial.print(numLed);
        Serial.print("     ");
                Serial.println( vReal[i]);


      for( int y = i * 10; y < (i * 10 + 10); y++){
        if(y < i * 10 + numLed)
          leds[y] = colorsArray[i];
        
        else
          leds[y] = CRGB::Black;  
      }
  }


    FastLED.show();
  }
  delay(50);
  }
