/*

  Honey
  Copyright 2018 - Matthew Carlin

  Effects class provides convenience methods to create visual effects.
*/

#ifndef HONEY_EFFECTS_H_
#define HONEY_EFFECTS_H_

#include <chrono>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "graphics.h"
#include "logic.h"

using namespace std;

namespace Honey {
  class Effects {
   public:

      Effects();

      enum STYLES {
        LINEAR,
        SIGMOID,
        CUBIC,
        RAMPDOWN,
        RAMPUP,
        SINEWAVE
      };

      const float sigmoid_steepness = 5;

      unordered_map<string, float> tween_starts;
      unordered_map<string, float> tween_ends;
      unordered_map<string, float> widths;

      bool check(string label);
      void destroyAllEffects();

      void makeTween(string label, float start_value, float end_value, float seconds);
      float tween(string label, int style);

      void makeShake(string label, int shake_width, float seconds);
      float shake(string label);

      void makeOscillation(string label, float oscillation_width, float period_in_seconds);
      float oscillation(string label);
  };

  extern Effects* effects;
}
#endif
