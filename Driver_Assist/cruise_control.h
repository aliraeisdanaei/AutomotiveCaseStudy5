#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

// #ifndef DRIVER_ASSIST
// #define DRIVER_ASSIST
// #endif

// #ifndef CRUISE_CONTROL
// #define CRUISE_CONTROL
// #endif

#include "../configuration.h"

class Cruise_Control {
private:
  bool cruise_control_on = false;

  // arbitrary acceleration
  static constexpr double drag_acc = 1.2;

public:
  double determine_drag_acc() { return drag_acc; }

  bool toggle_cruise_control(Car *car) {
    if (car->get_transmission_mode() != 'd') {
      car->give_warning("Cruise Control can only be toggled in drive mode");
    } else {
      this->cruise_control_on = this->cruise_control_on ? false : true;
    }
    return this->cruise_control_on;
  }

  void apply_cruise_control(Car *car, double unit_time_sec) {
    // car->accelerate(1, this->deterimine_dir_travel(), false,
    //  this->determine_drag_acc(), unit_time_sec);
    car->give_gas(determine_drag_acc() / car->max_acc);
  }
};