/*
 * 
 * 
 * -------------------------
 * DIFFERENTS SHIPS:
 * -------------------------
 * 
 * 1x Carrier     5 pixels
 * 1x Battleship  4 pixels
 * 2x Cruiser     3 pixels
 * 1x Destroyer   2 pixels
 * -------------------------
 * 
*/










///////////////////////////
// Cours Pascal pointeurs
//int a = 2;
//int& b = a;
//int* c = &a;
//b = 10;
//*c = 10;
//uint8_t *p_P1_carrier_x = &P1_carrier_x;
//uint8_t *p_P1_carrier_y = &P1_carrier_y;
//////////////////////////





#include <FastLED.h>


#define BUTTON_PIN A6
#define POT_PIN A0

// digital pins
#define SWITCH1_PIN 7
#define SWITCH2_PIN 3
#define BUZZER_PIN 13

// led strip/matrix parameter
#define LED_PIN  6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812

// width and height of matrix
const uint8_t MatrixWidth = 10;
const uint8_t MatrixHeight = 10;

#define NUM_LEDS (MatrixWidth * MatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);



//// ////////// DISPLAY SECTION ///////////
////// --------------------------------------
//#include <Adafruit_SSD1306.h>
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//Adafruit_SSD1306 mainDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
////// --------------------------------------




//////////////////////////////
//    SHIPS MAX LENGTH
//////////////////////////////
uint8_t max_carrier_length = 5;
uint8_t max_battleship_length = 4;
uint8_t max_cruiser_length = 3;
uint8_t max_destroyer_length = 2;
//////////////////////////////






//////////////////////////
//    PLAYER 1 SHIPS
//////////////////////////
uint8_t P1_carrier_x = 0;
uint8_t P1_carrier_y = 0;

uint8_t P1_battleship_x = 0;
uint8_t P1_battleship_y = 0;

uint8_t P1_cruiser1_x = 0;
uint8_t P1_cruiser1_y = 0;

uint8_t P1_cruiser2_x = 0;
uint8_t P1_cruiser2_y = 0;

uint8_t P1_destroyer_x = 0;
uint8_t P1_destroyer_y = 0;
//////////////////////////






//////////////////////////
//    PLAYER 2 SHIPS
//////////////////////////
uint8_t P2_carrier_x = 0;
uint8_t P2_carrier_y = 0;

uint8_t P2_battleship_x = 0;
uint8_t P2_battleship_y = 0;

uint8_t P2_cruiser1_x = 0;
uint8_t P2_cruiser1_y = 0;

uint8_t P2_cruiser2_x = 0;
uint8_t P2_cruiser2_y = 0;

uint8_t P2_destroyer_x = 0;
uint8_t P2_destroyer_y = 0;
//////////////////////////




void setup() {
  
  Serial.begin(115200);

//  mainDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3D);
//  mainDisplay.clearDisplay();
//  // Display initial print
//  mainDisplay.setTextSize(1);
//  mainDisplay.setTextColor(WHITE);
//  mainDisplay.setCursor(1, 1);
//  mainDisplay.println(" BATTLESHIP-PROJECT-V1.1");
//  mainDisplay.println("---------------------");
//  mainDisplay.println("Welcome to the game");
//  mainDisplay.println("Please visit game info");
//  mainDisplay.println("to know how to play.");
//  mainDisplay.println("---------------------");
//  mainDisplay.display();
//  delay(7000);
//  mainDisplay.clearDisplay();

  pinMode(SWITCH1_PIN, INPUT);
  pinMode(SWITCH2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // FastLED setup section 
  FastLED.clear();
  delay(300);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.show(); 
}



void loop(void) {
  
  FastLED.clear();

  //int switch1_state = digitalRead(SWITCH1_PIN);
  int switch2_state = digitalRead(SWITCH2_PIN);

  // print pixels positions for P1 and P2 in serial monitor
  printSerial_carrier_XY(switch2_state);
 
  if (switch2_state == 0) {
    move_carrier(BUTTON_PIN, BUZZER_PIN, P1_carrier_x, P1_carrier_y);
    //place_ship(BUTTON_PIN, BUZZER_PIN, P1_carrier_x, P1_carrier_y, 5, leds);
  }

  if (switch2_state == 1) {
    move_carrier(BUTTON_PIN, BUZZER_PIN, P1_battleship_x, P1_battleship_y);
  }



  // placing carrier
  for (int i = 0; i < max_carrier_length; i++) {
    if (XYsafe(P1_carrier_x, P1_carrier_y + i) == -1){
      return;
    }
    else  {
      leds[ XYsafe(P1_carrier_x, P1_carrier_y + i) ] = CRGB::Red;
    }
  }

  // placing battleship
  for (int i = 0; i < max_battleship_length; i++) {
    if (XYsafe(P1_battleship_x, P1_battleship_y + max_battleship_length - 1) == -1){
      return;
    }
    else  {
      leds[ XYsafe(P1_battleship_x, P1_battleship_y + i) ] = CRGB::Blue;
    }
  }
  
  //setting brightness by potentiometer
  brightness_by_potentiometer(POT_PIN, 50, 5, 250);
  
  FastLED.show();  
}













/////////////////////////////////////////////////////////////////////////////
//              move carrier pixel on RGB matrix
/////////////////////////////////////////////////////////////////////////////
//void place_ship(int button_pin, int buzzer_pin, uint8_t& carrier_x, uint8_t& carrier_y, int num_pixels, CRGB* const player_led_matrix) {
//
//  int buttonsValue = analogRead(button_pin);
//
//    // LEFT button is pressed
//    if ((buttonsValue == 0) || (buttonsValue <= 10))  {
//      if (XYsafe(carrier_x - 1, carrier_y) != -1)  {
//        carrier_x--;
//        //Serial.println("LEFT button is pressed");
//        tone(buzzer_pin, 5000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//      else  {
//        tone(buzzer_pin, 1000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//    }
//
//    // RIGHT button is pressed
//    if ((buttonsValue >= 735) && (buttonsValue <= 800)) {
//      if (XYsafe(carrier_x+1, carrier_y) != -1)   {
//        carrier_x++;
//        //Serial.println("RIGHT button is pressed");
//        tone(buzzer_pin, 5000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//      else  {
//        tone(buzzer_pin, 1000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//    }
//    
//    // UP button is pressed
//    if ((buttonsValue >= 475) && (buttonsValue <= 550)) {
//      if (XYsafe(carrier_x, carrier_y-1) != -1) {
//        carrier_y--;
//        //Serial.println("UP button is pressed");
//        tone(buzzer_pin, 5000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//      else  {
//        tone(buzzer_pin, 1000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//    }
//    
//    // DOWN button is pressed
//    if ((buttonsValue >= 625) && (buttonsValue <= 700)) {
//      if (XYsafe(carrier_x, carrier_y+1) != -1) {
//        carrier_y++;
//        //Serial.println("DOWN button is pressed");
//        tone(buzzer_pin, 5000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//      else  {
//        tone(buzzer_pin, 1000);
//        delay(200);
//        noTone(buzzer_pin);
//      }
//    } 
//
////    for (int i = 0; i < num_pixels; i++) {
////      if (XYsafe(carrier_x, carrier_y + i) == -1){
////        return;
////      }
////      else
////        player_led_matrix[ XYsafe(carrier_x,carrier_y + i) ] = CRGB::Red;
////    }
//  
//  }
/////////////////////////////////////////////////////////////////














////////////////////////////////////////
void printSerial_carrier_XY(int buttonvalue){
   Serial.print("P1_carrier_x: ");
   Serial.print(P1_carrier_x);
   Serial.print("      P1_carrier_y: ");
   Serial.print(P1_carrier_y);
   Serial.print("      P2_carrier_x: ");
   Serial.print(P2_carrier_x);
   Serial.print("      P2_carrier_y: ");
   Serial.print(P2_carrier_y);
   Serial.print("      Switch1_value: ");
   Serial.println(buttonvalue);
}
////////////////////////////////////////










////////////////////////////////////////////
//void printTurnToPlay(int player_number, Adafruit_SSD1306 mainDisplay){
//  mainDisplay.clearDisplay();
//  mainDisplay.setTextSize(1);
//  mainDisplay.setTextColor(WHITE);
//  mainDisplay.setCursor(1, 1);
//  mainDisplay.print("----INSTRUCTION----");
//  mainDisplay.print("Player: ");
//  mainDisplay.println(player_number);
//  mainDisplay.println(" your turn to play ");
//  // Display and clear buffer
//  mainDisplay.display();
//}
////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////
//              move carrier pixel on RGB matrix
/////////////////////////////////////////////////////////////////////////////
void move_carrier(int button_pin, int buzzer_pin, uint8_t& carrier_x, uint8_t& carrier_y) {

  int buttonsValue = analogRead(button_pin);

    // LEFT button is pressed
    if ((buttonsValue == 0) || (buttonsValue <= 10))  {
      if (XYsafe(carrier_x - 1, carrier_y) != -1)  {
        carrier_x--;
        //Serial.println("LEFT button is pressed");
        tone(buzzer_pin, 5000);
        delay(200);
        noTone(buzzer_pin);
      }
      else  {
        tone(buzzer_pin, 1000);
        delay(200);
        noTone(buzzer_pin);
      }
    }

    // RIGHT button is pressed
    if ((buttonsValue >= 735) && (buttonsValue <= 800)) {
      if (XYsafe(carrier_x+1, carrier_y) != -1)   {
        carrier_x++;
        //Serial.println("RIGHT button is pressed");
        tone(buzzer_pin, 5000);
        delay(200);
        noTone(buzzer_pin);
      }
      else  {
        tone(buzzer_pin, 1000);
        delay(200);
        noTone(buzzer_pin);
      }
    }
    
    // UP button is pressed
    if ((buttonsValue >= 475) && (buttonsValue <= 550)) {
      if (XYsafe(carrier_x, carrier_y-1) != -1) {
        carrier_y--;
        //Serial.println("UP button is pressed");
        tone(buzzer_pin, 5000);
        delay(200);
        noTone(buzzer_pin);
      }
      else  {
        tone(buzzer_pin, 1000);
        delay(200);
        noTone(buzzer_pin);
      }
    }
    
    // DOWN button is pressed
    if ((buttonsValue >= 625) && (buttonsValue <= 700)) {
      if (XYsafe(carrier_x, carrier_y+1) != -1) {
        carrier_y++;
        //Serial.println("DOWN button is pressed");
        tone(buzzer_pin, 5000);
        delay(200);
        noTone(buzzer_pin);
      }
      else  {
        tone(buzzer_pin, 1000);
        delay(200);
        noTone(buzzer_pin);
      }
    } 
  }
///////////////////////////////////////////////////////////////











// CURRENTLY NOT USING THIS FUNCTION
///////////////////////////////////////////////////////////////////
void buzzer_function(int buzzer_pin, int frequency, int delay_time) {
  tone(buzzer_pin, frequency);
  delay(delay_time);
  noTone(buzzer_pin);
}
///////////////////////////////////////////////////////////////////











/////////////////////////////////////////////////////
// a function to set brightness with potentiometer
/////////////////////////////////////////////////////
void brightness_by_potentiometer(int potentiometer_pin, int max_brightness, int max_volts, int max_mA){

  int potValue = analogRead(POT_PIN);         
  int brightness = map(potValue, 0, 1023, 0, max_brightness);
  
  FastLED.setMaxPowerInVoltsAndMilliamps(max_volts, max_mA);

  FastLED.setBrightness(brightness);
  FastLED.show();
}
/////////////////////////////////////////////////










////////////////////////////////////////////////////////
//      coordinates functions included in FastLED
////////////////////////////////////////////////////////
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  i = (y * MatrixWidth) + x;
  return i;
}

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= MatrixWidth)
    return -1;
  
  if( y >= MatrixHeight)
    return -1;
  
  return XY(x,y);
}
////////////////////////////////////////////////////////





//  char buffer[40];
//  sprintf(buffer, "P1_carrier_x: %d;     P1_carrier_y: %d;     P2_carrier_x: %d;     P2_carrier_y: %d;\n", P1_carrier_x, P1_carrier_y, P2_carrier_x, P2_carrier_y);
//  Serial.print(buffer);

//  Serial.print("P1_carrier1: ", P1_carrier_x);
//  Serial.println("P1_carrier_y: ", P1_carrier_y);


//leds[i] = CRGB(random(0, 255), random(0, 255), random(0, 255));
