/* SmoothLED library by Austin J Kurpuis
 */

#include "SmoothLED.h"

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

int SmoothLED::correctGamma(int color) {
  return pgm_read_byte(&gamma8[color]);
}

// Handles updating the color over time, should be called in the loop
void SmoothLED::update() {
  if (active) {
    if((millis() - lastUpdate) > interval) { // time to update
      lastUpdate = millis();
      // Calculate linear interpolation between startColor and endColor
      // Optimise order of operations to minimize truncation error
      uint8_t red = ((getRed(startColor) * (totalSteps - index)) + (getRed(endColor) * index)) / totalSteps;
      uint8_t green = ((getGreen(startColor) * (totalSteps - index)) + (getGreen(endColor) * index)) / totalSteps;
      uint8_t blue = ((getBlue(startColor) * (totalSteps - index)) + (getBlue(endColor) * index)) / totalSteps;

      currentColor = Color(red, green, blue);

      writeColorToStrip(currentColor);
      show();
      increment();
    }
  }
}

// Start fade to a new color
void SmoothLED::set(int red, int green, int blue) {
  active = true;
  startColor = currentColor;
  endColor = Color(red, green, blue);
  index = 0;
}

// Increment the index and reset at the end
void SmoothLED::increment() {
  index++;
  if (index > totalSteps) {
    index = 0;
    onComplete();
  }
}

void SmoothLED::onComplete() {
  active = false;
}

// Set all pixels to a color (synchronously)
void SmoothLED::writeColorToStrip(uint32_t color) {
  for (int i = 0; i < numPixels(); i++) {
    setPixelColor(i, color);
  }
  show();
}

// Returns the red component of a 32-bit color
uint8_t SmoothLED::getRed(uint32_t color) {
  return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t SmoothLED::getGreen(uint32_t color) {
  return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t SmoothLED::getBlue(uint32_t color) {
  return color & 0xFF;
}
