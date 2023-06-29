void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}





// /////////////////////////////////////////////////////////////////////////////////
void blueHueRandomFunction(int leds, int NUM_LEDS)  {
    // create a new pixel for led[0]
    leds[0] = CHSV(160, random8(), random8(100, 255));
  

    // copy each pixel to the next one, starting at the far end
    // thereby moving the pattern along the strip
    for (int i = NUM_LEDS-1; i > 0; i--)  {
      leds[i] = leds[i - 1];
    }
}
// /////////////////////////////////////////////////////////////////////////////////










// /////////////////////////////////////////////////////////////////////////////////
void LED_MOVING_BACK_AND_FORTH(float ledColor, unsigned long delayPeriod, int totalLed)    {
  
  FastLED.clear();
  
  for (int i = 0; i < totalLed; i++) {
    leds[i] = ledColor;
    FastLED.show();
    delay(delayPeriod);
    leds[i] = CRGB::Black;
  }  

  for (int i = totalLed-1; i >= 0; i--) {
    leds[i] = ledColor;
    FastLED.show();
    delay(delayPeriod);
    leds[i] = CRGB::Black;
  }  
}
// /////////////////////////////////////////////////////////////////////////////////















// /////////////////////////////////////////////////////////////////////////////////
void LED_MOVING_ALONG(float ledColor, unsigned long delayPeriod, int totalLed)    {
  
  FastLED.clear();
  
  for (int i = 0; i < totalLed; i++) {
    leds[i] = ledColor;
    FastLED.show();
      
    leds[i] = CRGB::Black;;
    delay(delayPeriod);
  }  
}
// /////////////////////////////////////////////////////////////////////////////////















// /////////////////////////////////////////////////////////////////////////////////
void LED_STACKING(float ledColor, unsigned long delayPeriod, int totalLed)    {
  
  FastLED.clear();
  
  for (int i = 0; i < totalLed; i++) {
    leds[i] = ledColor;
    FastLED.show();
    delay(delayPeriod);
    leds[i] = CRGB::Black;

    if (i == totalLed)  {
      leds[i] = ledColor; 
      FastLED.show();  
      delay(delayPeriod);
    }
  }  

  
}
// /////////////////////////////////////////////////////////////////////////////////











// /////////////////////////////////////////////////////////////////////////////////
void MULTIPLE_LED_MOVING_ALONG(unsigned long delayPeriod, int totalLed)  {

   for (int i = 0; i < totalLed; i++) {
    leds[i]  = CRGB::Red;
    if (i >= 5) {
      leds[i - 5] = CRGB::Yellow;
    }
    if (i >= 10) {
      leds[i - 10] = CRGB::Green;
    }
    if (i >= 15) {
      leds[i - 15] = CRGB::Cyan;
    }
    if (i >= 20) {
      leds[i - 20] = CRGB::Blue;
    }
    if (i >= 25) {
      leds[i - 25] = CRGB::Magenta;
    }
    FastLED.show();
    leds[i] = CRGB::Black;
    
    if (i >= 5) {
      leds[i - 5] = CRGB::Black;
    }
    if (i >= 10) {
      leds[i - 10] = CRGB::Black;
    }
    if (i >= 15) {
      leds[i - 15] = CRGB::Black;
    }
    if (i >= 20) {
      leds[i - 20] = CRGB::Black;
    }
    if (i >= 25) {
      leds[i - 25] = CRGB::Black;
    }
    delay(delayPeriod);
  }

}
// /////////////////////////////////////////////////////////////////////////////////













// /////////////////////////////////////////////////////////////////////////////////
byte r = 0; //start positions
byte g = 4;
byte b = 9;


void TEST(byte maxCount)
{
  
  /* ---------------------
 *  SECTION IN SETUP()
 *  --------------------
 * Serial.begin(115200);
 * while (!Serial);
 * Serial.println("r\tg\tb");
 * ---------------------
 */

 
 
  Serial.print(r);
  Serial.print("\t");
  Serial.print(g);
  Serial.print("\t");
  Serial.println(b);
  r++;
  if (r > maxCount)
  {
    r = 0;
  }
  g++;
  if (g > maxCount)
  {
    g = 0;
  }
  b++;
  if (b > maxCount)
  {
    b = 0;
  }
  delay(1000);
}
// /////////////////////////////////////////////////////////////////////////////////















// /////////////////////////////////////////////////////////////////////////////////
//Leds chase forwards along strip
int ledCount = 0;


void LED_CHASING()  {
  
  bool ledsAllOff = true;
  
  //shift the entire LED strip one LED position towards the end of the strip
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
    //check to see if the current LED is off (Black)
    if ((leds[i].red != 0) || (leds[i].green != 0) || (leds[i].blue != 0)) {
      ledsAllOff = false;  //this will be set false if any LED is not Black
    }
  }
  
  if (r == ledCount) {
    leds[0] = CRGB::Red;
    ledsAllOff = false;
  }
  
  else if (g == ledCount) {
    leds[0] = CRGB::Green;
    ledsAllOff = false;
  }
  
  else if (b == ledCount) {
    leds[0] = CRGB::Blue;
    ledsAllOff = false;
  }
  
  else {
    leds[0] = CRGB::Black;
  }
  
  FastLED.show();
  
  //restart the pattern when it has completely shifted through the LED strip
  if (ledsAllOff == true) {
    ledCount = 0;
  } else {
    ledCount++;
  }
  
  delay(delayPeriod);
}
// /////////////////////////////////////////////////////////////////////////////////
