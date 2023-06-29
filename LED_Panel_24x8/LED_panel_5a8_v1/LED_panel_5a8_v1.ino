








#include <FastLED.h>



#define LED_DATA_PIN 3    // this pin is for the main matrix led data pin
#define LED_MODE_PIN 4    //this pin is for 1 led to show the mode we are on

#define BUTTON_PIN A0     //this pin is for the button
#define MICROPHONE_PIN A1 //analog input of the microphone

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B












// Params for width and height
const uint8_t kMatrixWidth = 20;
const uint8_t kMatrixHeight = 6;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
const bool    kMatrixVertical = false;





//----------------------------------------------
/* DEFINING MODE FOR LED MATRIX
 * MODE 0 --> Preinstalled patern on LED matrix
 * MODE 1 --> Ukraine Flag (bottom blue and top yellow)
 * MODE 2 --> Full spectrum band analyzer (FFT)
 */
//----------------------------------------------

int MODE = 0



//----------------------------------------------
/* DEFINING COLOR ONLY FOR FFT MODE
 * --> CREATE A FULL COLOR PALETTE AND CHANGE COLOR ACCORDINGLY
 */
//----------------------------------------------
int color = 0;



// MAX VOLTAGE AND MAX CURRENT FOR THE MATRIX
int BRIGHTNESS = 0;
int MAX_VOLTAGE = 5;
float MAX_MILLI_AMPS = 1500;






#define NUM_LEDS (kMatrixWidth * kMatrixHeight)       //total of 120 leds

CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);









void setup() {

  Serial.begin(9600);



  
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  // make sure the matrix doesnt go over 1.5 Amps
  FastLED.setMaxPowerInVoltsAndMilliamps(MAX_VOLTAGE, MAX_MILLI_AMPS);



    pinMode(BUTTON_PIN, INPUT);
  pinMode(MICROPHONE_PIN, INPUT);

  pinMode(LED_DATA_PIN, OUTPUT);
  pinMode(LED_MODE_PIN, OUTPUT);

  FastLED.show();

}











void loop()
{

  int brightnes = map(buttonValue, 0, 1023, 0, max_brightness);
  



  
  

  FastLED.setBrightness(brightness);
   
  FastLED.show();
}







/////////////////////////////////////////////////////////////////////////////
//              move carrier pixel on RGB matrix
/////////////////////////////////////////////////////////////////////////////
void button_values(int button_pin, int &mode, int &color, int &brightness_in_milliAmps, int max_milli_amps) {

  int buttonsValue = analogRead(button_pin);


    // MODE button is pressed
    if ((buttonsValue == 0) || (buttonsValue <= 10))  {
      if (mode < 3) {
          mode += 1;  
          delay(200);
      }
      else  {
          mode = 0;
          delay(200);
      }
    }


    // COLOR button is pressed
    if ((buttonsValue >= 735) && (buttonsValue <= 800)) {
      if (color < 3) {
          color += 1;  
      }  
      else  {
        color = 0;
        delay(200);
      }
    }
    
    // BRIGHTNESS UP button is pressed
    if ((buttonsValue >= 475) && (buttonsValue <= 550)) {
      if (brightness_in_milliAmps < max_milli_amps) {
        brightness_in_milliAmps += 25; 
        delay(200);
      }
      else  {
        brightness_in_milliAmps = max_milli_amps;
        delay(200);
      }
    }
    
    // BRIGHTNESS DOWN button is pressed
    if ((buttonsValue >= 625) && (buttonsValue <= 700)) {
      if (brightness_in_milliAmps > 0) {
        brightness_in_milliAmps -= 25;
        delay(200);
      }
      else  {
        brightness_in_milliAmps = max_milli_amps;
        delay(200);
      }
    } 
  }
///////////////////////////////////////////////////////////////














/////////////////////////////////////////////////////
// a function to set brightness with potentiometer
/////////////////////////////////////////////////////
void brightness_by_button(int button_pin, int max_brightness, int max_volts, int max_mA){

  int buttonValue = analogRead(button_pin);         
  int brightness = map(buttonValue, 0, 1023, 0, max_brightness);
  
  FastLED.setMaxPowerInVoltsAndMilliamps(max_volts, max_mA);

  FastLED.setBrightness(brightness);
  FastLED.show();
}
/////////////////////////////////////////////////









void DrawOneFrame( uint8_t startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  uint8_t lineStartHue = startHue8;
  for( uint8_t y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    uint8_t pixelHue = lineStartHue;      
    for( uint8_t x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}








uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}








uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
    }
  }

  if( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
      }
    }
  }
  
  return i;
}
