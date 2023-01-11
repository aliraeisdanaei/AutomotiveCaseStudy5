#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <unistd.h>

#include "speaker_use.h"

using namespace std;

#ifndef SPEAKER
#define SPEAKER
#endif

class Speaker {
private:
  // true iff lockeded
  bool locked;
  Speaker_Use *current_use = (Speaker_Use *)malloc(sizeof(Speaker_Use));
  thread *current_use_thread = (thread *)malloc(sizeof(thread));

  void set_current_use(Speaker_Use *use);
  void unset_current_use();
  bool add_use(Speaker_Use *use);

public:
  Speaker() { this->locked = false; }

  ~Speaker() {
    delete this->current_use;
    delete this->current_use_thread;
  }

  /**
   * Returns true iff the use was added onto the speaker
   */
  bool add_use(uint priority, string name, use_func_type use_func,
               kill_func_type kill_func) {

    Speaker_Use *use = new Speaker_Use(priority, name, use_func, kill_func);
    return add_use(use);
  }
};