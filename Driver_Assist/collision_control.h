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

#ifndef COLLISION_CONTROL
#define COLLISION_CONTROL
#endif

class Collision_Control {
private:
public:
  // buffer time in seconds of buffer between current stoping capability to
  // obstacle
  static constexpr double obstacle_buffer_time = 4;

  Collision_Control() {}
  ~Collision_Control() {}

  static double get_pos_obstacle() {
    const double obstacle_position = 500;
    return obstacle_position;
  }

  static double get_dist_obstacle(double current_position) {
    return get_pos_obstacle() - current_position;
  }

  static bool collided(double current_position) {
    return get_dist_obstacle(current_position) < 0;
  }

  static bool close_obstacle(double current_position, double speed_met_sec,
                             double max_break_acc) {
    double dist = get_dist_obstacle(current_position);
    const double buffer_dist = obstacle_buffer_time * speed_met_sec;
    dist -= buffer_dist;

    // kinematic formula for distance travelled given acceleration
    // v^2 = v0^2 + 2a * disp
    // distance needed to stop
    // float break_coeff = car->deterimine_dir_travel() ? -1 : 1;
    double dist_needed =
        abs((0 - speed_met_sec * speed_met_sec) / (2 * max_break_acc));
    return dist_needed > dist;
  }
};