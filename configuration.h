#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <unistd.h>

using namespace std;

#ifndef CONFIGURATION
#define CONFIGURATION
#endif

#ifndef DRIVER_ASSIST
#define DRIVER_ASSIST
#endif

#ifndef COLLISION_CONTROL
#define COLLISION_CONTROL
class Collision_Control;
// #include "Driver_Assist/collision_control.h"
#endif

#ifndef CRUISE_CONTROL
#define CRUISE_CONTROL
class Cruise_Control;
// #include "Driver_Assist/cruise_control.h"
#endif

#ifndef SPEAKER
#define SPEAKER
// class Speaker;
// #include "Speaker/speaker.h"
// #include "Speaker/speaker_use.h"
#endif

#ifndef MUSIC_PLAYER
#define MUSIC_PLAYER
class Music_Player;
// #include "Music_Player/music_player.h"
#endif

#ifndef CAR
#define CAR

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

  // function declerations
  bool deterimine_dir_travel();
  void accelerate(double rate, bool forward, bool brake, double max,
                  double accel_time);

public:
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

  double get_speed();
  double get_speed_kmph();
  string get_model();
  int get_year();
  char get_transmission_mode();
  bool get_on();
  double get_position_x();

  void move_car();
  static void static_move_car(Car *car) { car->move_car(); }

  void give_gas(double gas_power);
  void brake(double brake_power);
  int change_transmission(int trans_desired);
  bool start();
  bool turnoff();
  void give_warning(string warning_msg);
  void write_car(bool clear);
};
#endif