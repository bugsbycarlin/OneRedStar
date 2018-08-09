/*

  Honey
  Copyright 2018 - Matthew Carlin

  Logic class provides game logic convenience functions.
*/

#ifndef HONEY_LOGIC_H_
#define HONEY_LOGIC_H_

#include <chrono>
#include <string>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

namespace Honey {
  class Logic {
   public:
      unordered_map<string, unsigned long> time_markers;
      unordered_map<string, float> duration_markers;

      unordered_map<string, int> transient_counter_values;

      Logic();

      // Remove a label from all logic
      void remove(string label);

      // Marks down the time for a label
      void markTime(string label);

      // Marks the intended duration of a label
      void markDuration(string label, float seconds);

      // Tells the time in seconds since a label was marked
      float timeSince(string label);

      // Remind me of the intended duration of the label
      float duration(string label);

      // Locks this label for this many seconds.
      void makeTimeLock(string label, float seconds);

      // Tells you if this label is still locked.
      bool isTimeLocked(string label);

      // Make a counter for this label that lasts for this many seconds.
      void makeTransientCounter(string label, float seconds);

      // Increment a transient counter if it's still valid.
      void incrementTransientCounter(string label, int value);

      // Give the value of a transient counter, or 0 if it's no longer valid.
      int transientCounterValue(string label);

      // Get a random integer between start and end
      int randomInt(int start, int end);
  };

  extern Logic* logic;
}
#endif
