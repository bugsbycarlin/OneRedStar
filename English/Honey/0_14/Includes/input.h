/*

  Honey
  Copyright 2018 - Matthew Carlin

  Input class wraps SDL input methods to create an event handling system.
*/

#ifndef HONEY_INPUT_H_
#define HONEY_INPUT_H_

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "logic.h"

using namespace std;

namespace Honey {
  class Input {
   public:
      unordered_map<string, string> action_to_key;

      unordered_map<string, bool> down;
      unordered_map<string, bool> up;
      unordered_map<string, int> pressed;

      Input();

      void processInput();
      string translateToKey(SDL_Event event);

      int keyPressed(string key);
      bool keyDown(string key);
      bool keyUp(string key);
      bool threeQuickKey(string key);

      int actionPressed(string action);
      bool actionDown(string action);
      bool actionUp(string action);
      bool threeQuickAction(string action);

      void addActionKey(string action, string key);
      void deleteAction(string action);

      ~Input();
  };

  extern Input* input;
}
#endif
