#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <unistd.h>

// #ifndef SPEAKER
// #define SPEAKER
// #endif

#include "../configuration.h"

#include "speaker_use.h"

using namespace std;

class Speaker {
private:
  // true iff locked
  bool lock;
  Speaker_Use *current_use = (Speaker_Use *)malloc(sizeof(Speaker_Use));
  thread *current_use_thread = (thread *)malloc(sizeof(thread));

  void set_current_use(Speaker_Use *use);
  bool add_use(Speaker_Use *use);

public:
  Speaker() { this->lock = false; }

  ~Speaker() {
    delete this->current_use;
    delete this->current_use_thread;
  }

  /**
   * Returns true iff the use was added onto the speaker
   */
  bool add_use(bool critical_priority, string name, use_func_type use_func,
               kill_func_type kill_func) {

    Speaker_Use *use =
        new Speaker_Use(critical_priority, name, use_func, kill_func);
    return add_use(use);
  }
};