#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

#include "configuration.h"
#ifndef CAR
#define CAR
#endif

#ifdef SPEAKER
#include "Speaker/speaker.h"
#endif

#ifdef COLLISION_CONTROL
#include "Driver_Assist/collision_control.h"
#endif

#ifdef CRUISE_CONTROL
#include "Driver_Assist/cruise_control.h"
#endif

#ifdef MUSIC_PLAYER
#include "Music_Player/music_player.h"
#endif

using namespace std;

class Car {
private:
  // all internal units are in meters per second
  double speed = 0;
  bool on = false;
  // tranision modes p, n, d, r
  char transition_mode = 'p';
  double position_x = 0;
  string model;
  int year;

  static constexpr double unit_time_sec = 5.0 / 1000;
  static constexpr double accel_time = 0.5;

  const double max_break_acc = 9.8;
  const double max_acc = 5.6; // 100 km in 5 sec --> 20km in 1 sec acceleration
  const double max_speed = 69.5; // max speed 250 kmph --> 69.445mps
  static constexpr double drag_acc = 1.2;

  void accelerate(double rate, bool forward, bool brake, double max,
                  double accel_time);
  bool deterimine_dir_travel();
  double determine_drag_acc();

#ifdef SPEAKER
  Speaker *speaker = new Speaker();
#endif

#ifdef COLLISION_CONTROL
  Collision_Control *collision_control = new Collision_Control();
#endif

#ifdef CRUISE_CONTROL
  Cruise_Control *cruise_control = new Cruise_Control();
#endif

#ifdef MUSIC_PLAYER
  Music_Player *music_player = new Music_Player();
#endif

#ifdef COLLISION_CONTROL
  static void use_collision_control(Car *car) {
    while (!Collision_Control::collided(car->position_x)) {
      if (Collision_Control::close_obstacle(car->position_x, car->speed,
                                            car->max_break_acc)) {
#ifdef CRUISE_CONTROL
        car->cruise_control->turnoff();
#endif
        car->give_warning("Obstacle dangerously close. Brake now.", 2);
      }
    }
  }
#endif

#ifdef CRUISE_CONTROL
  static void use_cruise_control(Car *car) {
    car->give_gas(car->cruise_control->get_gas_power(car->determine_drag_acc(),
                                                     car->max_acc),
                  unit_time_sec);
  }
#endif

public:
  Car(string model, int year) {
    this->model = model;
    this->year = year;
  }

  ~Car() {
#ifdef SPEAKER
    delete speaker;
#endif
  }

  double get_speed() { return mpsec_to_kmph(this->speed); }
  string get_model() { return this->model; }
  int get_year() { return this->year; }
  char get_transmission_mode() { return this->transition_mode; }
  bool get_on() { return this->on; }
  double get_position_x() { return this->position_x; }

  static double mpsec_to_kmph(double mpsec) { return mpsec * 3.6; }

#ifdef CRUISE_CONTROL
  void toggle_cruise_control() {
    this->cruise_control->toggle_cruise_control(this->transition_mode,
                                                this->get_speed());
  }
#endif

#ifdef MUSIC_PLAYER
  void play_music() {
    cout << "play music" << endl;
#ifdef SPEAKER
    this->speaker->add_use(0, music_player->song1, music_player->play_mpv,
                           music_player->kill_mpv);
#endif
  }
#endif

  void move_car() {
#ifdef COLLISION_CONTROL
    thread collision_control_thread(use_collision_control, this);
#endif

    while (true) {
      this->position_x += this->speed * unit_time_sec;
      this_thread::sleep_for(chrono::milliseconds((int)(unit_time_sec * 1000)));

      // apply drag to the car
      accelerate(1, this->deterimine_dir_travel(), true, this->drag_acc,
                 unit_time_sec);

#ifdef CRUISE_CONTROL
      if (this->cruise_control->get_on()) {
        use_cruise_control(this);
      }
#endif

#ifdef COLLISION_CONTROL
      if (this->collision_control->collided(this->position_x)) {
        this->speed = 0;
        give_warning("You have crashed", 3);
        collision_control_thread.join();
        break;
      }
#endif
    }
  }

  static void static_move_car(Car *car) { car->move_car(); }

  void give_gas(double gas_power, double accel_time) {
    if (this->transition_mode == 'd') {
      accelerate(gas_power, true, false, this->max_acc, accel_time);
    } else if (this->transition_mode == 'r') {
      accelerate(gas_power, false, false, this->max_acc, accel_time);
    } else {
      give_warning(
          "You cannot give gas when you are not in drive or in reverse", 1);
    }
  }
  void give_gas(double gas_power) { give_gas(gas_power, Car::accel_time); }

  void brake(double brake_power) {
    const double brake_time = 0.5;
    accelerate(brake_power, this->deterimine_dir_travel(), true,
               this->max_break_acc, brake_time);

#ifdef CRUISE_CONTROL
    this->cruise_control->turnoff();
#endif
  }

  int change_transmission(int trans_desired) {
    if (!this->on) {
      give_warning("You cannot change the transmission when the car is not on",
                   1);
    } else {
      if (this->speed == 0) {
        this->transition_mode = trans_desired;

#ifdef CRUISE_CONTROL
        this->cruise_control->turnoff();
#endif

      } else {
        give_warning(
            "You cannot change the transmission when the car is moving", 1);
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
      give_warning("Cannot turn off car when speed is greater than 0", 1);
    } else if (this->transition_mode != 'p') {
      give_warning("You cannot turn off the car when you are not in park", 1);
    } else {
      this->on = false;
    }
    return this->on;
  }

  void give_warning(string warning_msg, uint priority) {
    // cout << warning_msg << '\n';
#ifdef SPEAKER
    // give_speaker_warning(warning_msg);
    // Speaker_Use *audio_warning =
    // new Speaker_Use(true, warning_msg, &Speaker_Use::give_speaker_warning,
    // &Speaker_Use::kill_speaker_warning);
    this->speaker->add_use(priority, warning_msg,
                           &Speaker_Use::give_speaker_warning,
                           &Speaker_Use::kill_speaker_warning);
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
#ifdef CRUISE_CONTROL
    cout << "Cruise control: " << this->cruise_control->get_on() << '\n';
#endif
    cout << '\n';
    cout << "Speed: " << this->get_speed() << '\n';
    cout << "Position x: " << this->position_x << '\n';

#ifdef COLLISION_CONTROL
    cout << "Distance to Obstacle: "
         << Collision_Control::get_dist_obstacle(this->position_x) << ' '
         << "meters" << '\n';
#endif
  }
};
