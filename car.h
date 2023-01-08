#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#ifndef CAR
#define CAR

using namespace std;

class Car {
private:
  // all internal units are in meters per second
  double speed = 0;
  bool on = false;

  bool cruise_control_on = false;

  // tranision modes p, n, d, r
  char transition_mode = 'p';

  // acceleration taken as reference from
  // https://copradar.com/chapts/references/acceleration.html
  const double max_break_acc = 9.8;

  // 100 km in 5 sec --> 20km in 1 sec acceleration
  const double max_acc = 5.6;

  // just took a gues
  const double drag_acc = 1.2;

  // max speed 250 kmph --> 69.445mps
  const double max_speed = 69.5;

  double position_x = 0;
  double obstacle_position = 5000;

  string model;
  int year;

  double mpsec_to_kmph(double mpsec);
  void accelerate(double rate, bool forward, bool brake, double max,
                  double accel_time);
  bool deterimine_dir_travel();
  double determine_drag_acc();

public:
  Car(string model, int year) {
    this->model = model;
    this->year = year;
  }

  ~Car() {}

  double get_speed() { return mpsec_to_kmph(this->speed); }

  string get_model() { return this->model; }

  int get_year() { return this->year; }

  char get_transmission_mode() { return this->transition_mode; }

  bool get_on() { return this->on; }

  double get_position_x() { return this->position_x; }

  bool toggle_cruise_control() {
    if (this->transition_mode != 'd') {
      give_warning("Cruise Control can only be toggled in drive mode");
    } else {
      this->cruise_control_on = this->cruise_control_on ? false : true;
    }
    return this->cruise_control_on;
  }

  void apply_cruise_control(double unit_time_sec) {
    accelerate(1, this->deterimine_dir_travel(), false,
               this->determine_drag_acc(), unit_time_sec);
  }

  void move_car() {
    while (true) {
      const double unit_time_sec = 5.0 / 1000;
      while (true) {
        this->position_x += this->speed * unit_time_sec;
        this_thread::sleep_for(
            chrono::milliseconds((int)(unit_time_sec * 1000)));

        // apply drag to the car
        accelerate(1, this->deterimine_dir_travel(), true, this->drag_acc,
                   unit_time_sec);

        if (this->cruise_control_on) {
          apply_cruise_control(unit_time_sec);
        }
      }
    }
  }

  void give_gas(double gas_power) {
    const double accel_time = 0.5;
    if (this->transition_mode == 'd') {
      accelerate(gas_power, true, false, this->max_acc, accel_time);
    } else if (this->transition_mode == 'r') {
      accelerate(gas_power, false, false, this->max_acc, accel_time);
    } else {
      give_warning(
          "You cannot give gas when you are not in drive or in reverse");
    }
  }

  void brake(double brake_power) {
    const double brake_time = 0.5;
    accelerate(brake_power, this->deterimine_dir_travel(), true,
               this->max_break_acc, brake_time);
    this->cruise_control_on = false;
  }

  int change_transmission(int trans_desired) {
    if (!this->on) {
      give_warning("You cannot change the transmission when the car is not on");
    } else {
      if (this->speed == 0) {
        this->transition_mode = trans_desired;
        this->cruise_control_on = false;
      } else {
        give_warning(
            "You cannot change the transmission when the car is moving");
      }
    }
    return this->transition_mode;
  }

  double get_dist_obstacle() {
    return this->obstacle_position - this->position_x;
  }

  bool start() {
    this->on = true;
    return this->on;
  }

  bool turnoff() {
    if (this->speed != 0) {
      string warning = "Cannot turn off car when speed is greater than 0";
      give_warning(warning);
    } else if (this->transition_mode != 'p') {
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

  void write_car(bool clear) {
    if (clear) {
      system("clear");
    }
    cout << "Model: " << this->model << '\n';
    cout << "Year: " << this->year << '\n';
    cout << "On: " << this->get_on() << '\n';
    cout << "Transmission: " << this->get_transmission_mode() << '\n';
    cout << "Cruise control: " << this->cruise_control_on << '\n';
    cout << '\n';
    cout << "Speed: " << this->get_speed() << '\n';
    cout << "Position x: " << this->position_x << '\n';
    cout << "Distance to Obstacle: " << this->get_dist_obstacle() << ' '
         << "meters" << '\n';
  }
};

#endif