#pragma once

/* SmoothLED library by Austin J Kurpuis
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

#include <neopixel.h>

// This is your main class that users will import into their application
class SmoothLED : public Adafruit_NeoPixel {
  public:
    /**
     * Constructor - calls base-class constructor to initialize strip
     */
    SmoothLED(uint16_t pixels, uint8_t pin, int intervalOverride = 5, int totalStepsOverride = 50)
    :Adafruit_NeoPixel(pixels, pin){
      interval = intervalOverride;
      totalSteps = totalStepsOverride;
    }

    // Member Variables:
    bool active;  // which pattern is running

    unsigned long interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t startColor;
    uint32_t endColor;
    uint32_t currentColor = Color(0, 0, 0);  // current color shown on the lights
    uint16_t totalSteps;  // total number of steps in the fade
    uint16_t index;  // current step within the pattern

    void update();
    void set(int red, int green, int blue);
    int correctGamma(int color);

  private:
    uint8_t getRed(uint32_t color);
    uint8_t getGreen(uint32_t color);
    uint8_t getBlue(uint32_t color);
    void writeColorToStrip(uint32_t color);
    void increment();
    void onComplete();
};
