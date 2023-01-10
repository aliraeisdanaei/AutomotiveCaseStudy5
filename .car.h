#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#ifndef CAR
#define CAR
#endif

#include "configuration.h"

// #ifdef SPEAKER
// #include "Speaker/speaker.h"
// #endif

// #ifdef COLLISION_CONTROL
// #include "Driver_Assist/collision_control.h"
// #endif

// #ifdef CRUISE_CONTROL
// #include "Driver_Assist/cruise_control.h"
// #endif

using namespace std;

class Car {
private:
  const double accel_time = 0.5;

  // all internal units are in meters per second
  double speed = 0;
  bool on = false;

  // tranision modes p, n, d, r
  char transition_mode = 'p';

  double position_x = 0;

  string model;
  int year;

  void accelerate(double rate, bool forward, bool brake, double max,
                  double accel_time);
  bool deterimine_dir_travel();

#ifdef SPEAKER
  // Speaker *speaker = (Speaker *)malloc(sizeof(Speaker));
  Speaker *speaker = new Speaker();
#endif

#ifdef COLLISION_CONTROL
  Collision_Control *collision_control = new Collision_Control();
#endif

#ifdef CRUISE_CONTROL
  Cruise_Control *cruise_control = new Cruise_Control();
#endif

}

public :
    // acceleration taken as reference from
    // https://copradar.com/chapts/references/acceleration.html
    const double max_break_acc = 9.8;

// 100 km in 5 sec --> 20km in 1 sec acceleration
const double max_acc = 5.6;

// max speed 250 kmph --> 69.445mps
const double max_speed = 69.5;

Car(string model, int year) {
  this->model = model;
  this->year = year;
}

~Car() {
#ifdef SPEAKER
  delete speaker;
#endif
}

static double mpsec_to_kmph(double mpsec) { return mpsec * 3.6; }
double get_speed() { return this->speed; }
double get_speed_kmph() { return mpsec_to_kmph(this->speed); }
string get_model() { return this->model; }
int get_year() { return this->year; }
char get_transmission_mode() { return this->transition_mode; }
bool get_on() { return this->on; }
double get_position_x() { return this->position_x; }

void move_car() {
#ifdef COLLISION_CONTROL
  Collision_Control::create_collision_cont_thread(this);
#endif

  while (true) {
    const double unit_time_sec = 5.0 / 1000;

    this->position_x += this->speed * unit_time_sec;
    this_thread::sleep_for(chrono::milliseconds((int)(unit_time_sec * 1000)));

    // apply drag to the car
    accelerate(1, this->deterimine_dir_travel(), true, this->drag_acc,
               unit_time_sec);

    if (this->cruise_control_on) {
      apply_cruise_control(unit_time_sec);
    }

#ifdef COLLISION_CONTROL
    if (Collision_Control::collided(this->position_x)) {
      this->speed = 0;
      give_warning("You have crashed");
      break;
    }
#endif
  }

  static void static_move_car(Car * car) { car->move_car(); }

  void give_gas(double gas_power) {
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

  // #ifdef SPEAKER
  //   static void speaker_warning(string warning_msg) {
  //     string espeak_cmd = "espeak \'warning " + warning_msg + '\'';
  //     system(espeak_cmd.c_str());
  //   }

  // #endif

  void give_warning(string warning_msg) {
    // cout << warning_msg << '\n';
#ifdef SPEAKER
    // give_speaker_warning(warning_msg);
    Speaker_Use *audio_warning =
        new Speaker_Use(true, warning_msg, &Speaker_Use::give_speaker_warning,
                        &Speaker_Use::kill_speaker_warning);
    this->speaker->add_use(audio_warning);
#endif
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
    cout << "Speed: " << this->get_speed_kmph() << '\n';
    cout << "Position x: " << this->position_x << '\n';
    cout << "Distance to Obstacle: " << this->get_dist_obstacle() << ' '
         << "meters" << '\n';
  }
};
