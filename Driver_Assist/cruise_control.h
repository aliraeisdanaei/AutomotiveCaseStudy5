#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "../configuration.h"

#ifndef DRIVER_ASSIST
#define DRIVER_ASSIST
#endif

#ifndef CRUISE_CONTROL
#define CRUISE_CONTROL
#endif

using namespace std;

class Cruise_Control {
private:
  bool on = false;

public:
  Cruise_Control() {}
  ~Cruise_Control() {}

  bool get_on() { return this->on; }
  void turnoff() { this->on = false; }

  bool toggle_cruise_control(char transition_mode) {
    if (transition_mode != 'd') {
      return false;
    } else {
      this->on = this->on ? false : true;
    }
    return true;
  }

  static double get_gas_power(double drag_acc, double max_acc) {
    return drag_acc / max_acc;
  }
};