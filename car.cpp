#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "configuration.h"

#include "Speaker/speaker.h"

using namespace std;

double Car::get_speed() { return this->speed; }
double Car::get_speed_kmph() { return mpsec_to_kmph(this->speed); }
string Car::get_model() { return this->model; }
int Car::get_year() { return this->year; }
char Car::get_transmission_mode() { return this->transition_mode; }
bool Car::get_on() { return this->on; }
double Car::get_position_x() { return this->position_x; }

void Car::move_car() {
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
}

void Car::give_gas(double gas_power) {
  if (this->transition_mode == 'd') {
    accelerate(gas_power, true, false, this->max_acc, accel_time);
  } else if (this->transition_mode == 'r') {
    accelerate(gas_power, false, false, this->max_acc, accel_time);
  } else {
    give_warning("You cannot give gas when you are not in drive or in reverse");
  }
}

void Car::brake(double brake_power) {
  const double brake_time = 0.5;
  accelerate(brake_power, this->deterimine_dir_travel(), true,
             this->max_break_acc, brake_time);
  this->cruise_control_on = false;
}

int Car::change_transmission(int trans_desired) {
  if (!this->on) {
    give_warning("You cannot change the transmission when the car is not on");
  } else {
    if (this->speed == 0) {
      this->transition_mode = trans_desired;
      this->cruise_control_on = false;
    } else {
      give_warning("You cannot change the transmission when the car is moving");
    }
  }
  return this->transition_mode;
}

bool Car::start() {
  this->on = true;

#ifdef SPEAKER
  speaker = new Speaker();
#endif

#ifdef COLLISION_CONTROL
  //   Collision_Control *collision_control = new Collision_Control();
  Collision_Control *collision_control;
#endif

#ifdef CRUISE_CONTROL
  //   Cruise_Control *cruise_control = new Cruise_Control();
  Cruise_Control *cruise_control;
#endif

  return this->on;
}

bool Car::turnoff() {
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

void Car::give_warning(string warning_msg) {
  // cout << warning_msg << '\n';
#ifdef SPEAKER
  // give_speaker_warning(warning_msg);
  Speaker_Use *audio_warning =
      new Speaker_Use(true, warning_msg, &Speaker_Use::give_speaker_warning,
                      &Speaker_Use::kill_speaker_warning);
  this->speaker->add_use(audio_warning);
#endif
}

void Car::write_car(bool clear) {
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

/**
 * double rate (0, 1] the level of power to accelarate the car forward for
 * 0.5s
 */
void Car::accelerate(double rate, bool forward, bool brake, double max,
                     double accel_time) {
  if ((this->transition_mode == 'p' || this->transition_mode == 'n') &&
      !brake) {
    give_warning("You cannot accelerate when the car is parked or in neutral");
  } else if (!(0 < rate && rate <= 1)) {
    give_warning("The power has to be in (0, 1]");
  } else {
    double accel = forward ? max * rate : -1 * max * rate;
    double delta_speed = accel * accel_time;
    if (brake) {
      // make sure the brake stops the speed to 0
      const double new_speed = this->speed - delta_speed;
      if (forward) {
        this->speed = new_speed < 0 ? 0 : new_speed;
      } else {
        this->speed = new_speed > 0 ? 0 : new_speed;
      }
    } else {
      if (abs(delta_speed + this->speed) < this->max_speed) {
        this->speed += delta_speed;
      }
    }
  }
}

bool Car::deterimine_dir_travel() {
  return this->transition_mode == 'd' ? true : false;
}