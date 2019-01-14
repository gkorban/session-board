#include <FastLED.h>
#include <math.h>       /* log */

#define NUM_LEDS 44
#define NUM_DIGITS 10
#define NUM_PLACES 3

#define DATA_PIN 6  // select the output pin for the LED data

#define SESSION_BUTTON_PIN 10
#define MATCH_UP_BUTTON_PIN 12
#define MATCH_DOWN_BUTTON_PIN 11

#define MAX_CONFIGS 6

#define SESSION_PLACE 2  // where to display the session digit
#define MATCH_TENS_PLACE 1 // where to display the match number tens digit
#define MATCH_ONES_PLACE 0 // where to display the match number ones digit

#define MAX_BRIGHTNESS 256
#define DIM_FACTOR 64 // between zero and 256

const char led_on = '1';
const char digit_configs[NUM_DIGITS][NUM_LEDS] = {
  "11111111111111111111111111111111111111000000", // 0
  "11111111111110000000000000000000000000000000", // 1
  "11111100000001111111111110000000111111111111", // 2
  "11111101111111111110000000000000111111111111", // 3
  "11111111111110000000000001111111000000111111", // 4
  "00000001111111111110000001111111111111111111", // 5
  "00000001111111111111111111111111111111111111", // 6
  "11111111111110000000000000000000111111000000", // 7
  "11111111111111111111111111111111111111111111", // 8
  "11111111111111111110000000111111111111111111"}; // 9

const CRGB session_color = CRGB::Blue;
const CRGB match_color = CRGB::Red;

int session = 1;
int match = 1;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 300;    // the debounce time; increase if the output flickers

CRGB leds[NUM_LEDS * NUM_PLACES];

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS * NUM_PLACES);
  
  pinMode(SESSION_BUTTON_PIN, INPUT_PULLUP);  
  pinMode(MATCH_UP_BUTTON_PIN, INPUT_PULLUP);  
  pinMode(MATCH_DOWN_BUTTON_PIN, INPUT_PULLUP);  
  
  Serial.println("done setup");
  
}

void loop() {
  readButtons();
    
  if (match > 99) {
    match = 1;
  }
  if (match < 1) {
    match = 99;
  }
  if (session > 9) {
    session = 1;
  }
  if (session < 1) {
    session = 9;
  }
  showSession();
  showMatch();

  delay(10);
}

void readButtons() {
  

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (digitalRead(SESSION_BUTTON_PIN) == LOW) {
      session++;
      lastDebounceTime = millis();
    }
    if (digitalRead(MATCH_UP_BUTTON_PIN) == LOW) {
      match++;
      lastDebounceTime = millis();
    }
    if (digitalRead(MATCH_DOWN_BUTTON_PIN) == LOW) {
      match--;
      lastDebounceTime = millis();
    }
  }
}

void showSession() {
  showDigit(SESSION_PLACE, session, session_color);
}

void showMatch() {
  int tens = (int) match/10;
  int ones = (int) match % 10;
  
  showDigit(MATCH_TENS_PLACE, tens, match_color);
  showDigit(MATCH_ONES_PLACE, ones, match_color);
}

void showDigit(int place, int digit, CRGB color) {
  for(int dot = 0; dot < NUM_LEDS; dot++) {
    if(digit_configs[digit][dot] == led_on) {
      leds[dot + (place * NUM_LEDS)] = color;
      leds[dot + (place * NUM_LEDS)].fadeLightBy(DIM_FACTOR);
    } else {
      leds[dot + (place * NUM_LEDS)] = CRGB::Black;
    }
  }
  FastLED.show();
}
