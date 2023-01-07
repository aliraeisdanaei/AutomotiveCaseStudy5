#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>

#include "car.h"

using namespace std;

double Car::mpsec_to_kmph(double mpsec) { return mpsec * 3.6; }

/**
 * double rate (0, 1] the level of power to accelarate the car forward for
 * 0.5s
 */
void Car::accelerate(double rate, bool forward, bool brake, double max,
                     double accel_time) {
  if (this->transition_mode == 0 || this->transition_mode == 1) {
    give_warning("You cannot accelerate when the car is parked or in neutral");
  } else if (!(0 < rate && rate <= 1)) {
    give_warning("The power has to be in (0, 1]");
  } else {
    double accel = forward ? max * rate : -1 * max * rate;
    double new_speed = this->speed + accel * accel_time;
    if (!brake) {
      if (abs(new_speed) < this->max_speed) {
        this->speed = new_speed;
      }
    } else {

      // make sure the brake stops the speed to 0
      if (forward) {
        this->speed = new_speed > 0 ? 0 : new_speed;
      } else {
        this->speed = new_speed < 0 ? 0 : new_speed;
      }
    }
  }
}
