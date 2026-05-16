/*
=====================================================

   Project : Arduino Rocket Launch System
   Channel : AmithLabs
   Created By : Amith Wijekoon

   YouTube :
   https://www.youtube.com/@AmithLabs

   Description:
   DIY cinematic rocket launch platform featuring
   OLED countdown animation, smoke generator,
   ignition control, and RGB rocket fire effect.

   © AmithLabs - All Rights Reserved

=====================================================
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// =====================================================
// OLED SETTINGS
// =====================================================
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =====================================================
// RELAY PINS
// =====================================================
#define IGNITION_PIN 3
#define SMOKE_PIN    2

// =====================================================
// RGB LED SETTINGS
// =====================================================
#define LED_PIN     6
#define NUM_LEDS    3

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// =====================================================
// SETUP
// =====================================================
void setup() {

  // ==========================================
  // RELAY OUTPUTS
  // ==========================================
  pinMode(IGNITION_PIN, OUTPUT);
  pinMode(SMOKE_PIN, OUTPUT);

  // Active LOW relay OFF
  digitalWrite(IGNITION_PIN, HIGH);
  digitalWrite(SMOKE_PIN, HIGH);

  // ==========================================
  // RGB LED START
  // ==========================================
  strip.begin();

  strip.setBrightness(140);

  strip.show();

  randomSeed(analogRead(A0));

  // ==========================================
  // OLED START
  // ==========================================
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    while (1);
  }

  display.clearDisplay();
  display.display();

  // ==========================================
  // COUNTDOWN
  // ==========================================
  countdown();

  // ==========================================
  // START SMOKE
  // ==========================================
  digitalWrite(SMOKE_PIN, LOW);

  // Wait 1 second
  delay(100);

  // ==========================================
  // LAUNCHING SCREEN
  // ==========================================
  launchingScreen();

  // ==========================================
  // STOP FIRE LEDs
  // ==========================================
  clearLEDs();

  // ==========================================
  // IGNITION
  // ==========================================
  ignitionScreen();

  digitalWrite(IGNITION_PIN, LOW);
  delay(500);
  digitalWrite(IGNITION_PIN, HIGH);

  // ==========================================
  // STOP SMOKE
  // ==========================================
  digitalWrite(SMOKE_PIN, HIGH);

  // ==========================================
  // LAUNCHED SCREEN
  // ==========================================
  launchedScreen();

  // ==========================================
  // SAFETY OFF
  // ==========================================
  digitalWrite(IGNITION_PIN, HIGH);
  digitalWrite(SMOKE_PIN, HIGH);

  clearLEDs();
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  // Keep everything OFF forever

  digitalWrite(IGNITION_PIN, HIGH);
  digitalWrite(SMOKE_PIN, HIGH);

  clearLEDs();

  delay(1000);
}

// =====================================================
// COUNTDOWN
// =====================================================
void countdown() {

  for (int num = 3; num >= 1; num--) {

    unsigned long startTime = millis();

    while (millis() - startTime < 1000) {

      display.clearDisplay();

      // ======================================
      // TOP ANIMATION
      // ======================================
      topAnimation();

      // ======================================
      // BIG NUMBER
      // ======================================
      display.setTextSize(5);
      display.setTextColor(SSD1306_WHITE);

      char txt[2];
      txt[0] = '0' + num;
      txt[1] = '\0';

      int16_t x1, y1;
      uint16_t w, h;

      display.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);

      int x = (SCREEN_WIDTH - w) / 2;
      int y = ((SCREEN_HEIGHT - h) / 2) + 5;

      display.setCursor(x, y);
      display.print(txt);

      display.display();

      delay(50);
    }
  }
}

// =====================================================
// TOP STRIP ANIMATION
// =====================================================
void topAnimation() {

  static int pos = 0;

  for (int i = 0; i < 8; i++) {

    int x = (pos + (i * 16)) % 128;

    display.fillRect(x, 0, 10, 10, SSD1306_WHITE);
  }

  pos += 4;

  if (pos >= 128) pos = 0;
}

// =====================================================
// LAUNCHING SCREEN
// =====================================================
void launchingScreen() {

  // About 9 sec progress
  for (int progress = 0; progress <= 128; progress++) {

    display.clearDisplay();

    // ======================================
    // PROGRESS BAR
    // ======================================
    display.drawRect(0, 0, 128, 10, SSD1306_WHITE);

    display.fillRect(0, 0, progress, 10, SSD1306_WHITE);

    // ======================================
    // TEXT
    // ======================================
    centerText("Launching", 2);

    display.display();

    // ======================================
    // FIRE EFFECT
    // ======================================
    fireEffect();

    delay(70);
  }
}

// =====================================================
// FIRE EFFECT
// =====================================================
void fireEffect() {

  for (int i = 0; i < NUM_LEDS; i++) {

    int r, g, b;

    int mode = random(5);

    // ======================================
    // FIRE COLOR PALETTE
    // ======================================
    if (mode == 0) {

      r = 180;
      g = 0;
      b = 0;
    }
    else if (mode == 1) {

      r = 255;
      g = 20;
      b = 0;
    }
    else if (mode == 2) {

      r = 255;
      g = 50;
      b = 0;
    }
    else if (mode == 3) {

      r = 255;
      g = 80;
      b = 0;
    }
    else {

      r = 255;
      g = 120;
      b = 0;
    }

    // ======================================
    // RANDOM FLICKER
    // ======================================
    int flicker = random(120, 255);

    r = (r * flicker) / 255;
    g = (g * flicker) / 255;
    b = (b * flicker) / 255;

    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  // ======================================
  // RANDOM HOT FLASH
  // ======================================
  if (random(100) > 95) {

    int led = random(NUM_LEDS);

    strip.setPixelColor(led, strip.Color(255, 180, 50));
  }

  strip.show();
}

// =====================================================
// CLEAR LEDs
// =====================================================
void clearLEDs() {

  for (int i = 0; i < NUM_LEDS; i++) {

    strip.setPixelColor(i, 0);
  }

  strip.show();
}

// =====================================================
// IGNITION SCREEN
// =====================================================
void ignitionScreen() {

  display.clearDisplay();

  display.fillRect(0, 0, 128, 10, SSD1306_WHITE);

  centerText("IGNITION", 2);

  display.display();
}

// =====================================================
// LAUNCHED SCREEN
// =====================================================
void launchedScreen() {

  for (int i = 0; i < 8; i++) {

    display.clearDisplay();

    if (i % 2 == 0) {

      display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
    }

    centerText("Launched", 2);

    display.display();

    delay(250);
  }
}

// =====================================================
// CENTER TEXT
// =====================================================
void centerText(const char* text, int size) {

  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2;

  display.setCursor(x, y);

  display.print(text);
}