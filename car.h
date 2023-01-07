#include <cstddef>
#include <iostream>
#include <string>

#ifndef CAR
#define CAR

using namespace std;

class Car {
private:
  // all internal units are in meters per second
  double speed = 0;
  bool on = false;

  // tranision modes
  // 0 park, 1 neutral, 2 drive, 3 reverse
  int transition_mode = 0;

  // acceleration taken as reference from
  // https://copradar.com/chapts/references/acceleration.html
  const double max_break_acc = 9.8;

  // 100 km in 5 sec --> 20km in 1 sec acceleration
  const double max_acc = 5.6;

  // max speed 250 kmph --> 69.445mps
  const double max_speed = 69.5;

  string model;
  int year;

  double mpsec_to_kmph(double mpsec);
  void accelerate(double rate, bool forward, bool brake, double max,
                  double accel_time);

public:
  Car(string model, int year) {
    this->model = model;
    this->year = year;
  }

  ~Car() {}

  double get_speed() { return mpsec_to_kmph(this->speed); }

  string get_model() { return this->model; }

  int get_year() { return this->year; }

  int get_transmission_mode() { return this->transition_mode; }

  void give_gas(double gas_power) {
    const double accel_time = 0.5;
    bool forward;
    if (this->transition_mode == 2) {
      forward = true;
    } else if (this->transition_mode == 3) {
      forward = false;
    } else {
      give_warning(
          "You cannot give gas when you are not in drive or in reverse");
    }
    accelerate(gas_power, forward, false, this->max_acc, accel_time);
  }

  void brake(double brake_power) {
    const double brake_time = 0.5;
    bool forward = this->transition_mode == 2 ? true : false;
    accelerate(brake_power, forward, true, this->max_break_acc, brake_time);
  }

  int change_transmission(int trans_desired) {
    if (!this->on) {
      give_warning("You cannot change the transmission when the car is not on");
    } else {
      if (this->speed == 0) {
        this->transition_mode = trans_desired;
      } else {
        give_warning(
            "You cannot change the transmission when the car is moving");
      }
    }
    return this->transition_mode;
  }

  bool start() {
    this->on = true;
    return this->on;
  }

  bool turnoff() {
    if (this->speed != 0) {
      string warning = "Cannot turn off car when speed is greater than 0";
      give_warning(warning);
    } else if (this->transition_mode != 0) {
      give_warning("You cannot turn off the car when you are not in park");
    } else {
      this->on = false;
    }
    return this->on;
  }

  bool give_warning(string warning) {
    cout << warning << '\n';
    return true;
  }
};

#endif