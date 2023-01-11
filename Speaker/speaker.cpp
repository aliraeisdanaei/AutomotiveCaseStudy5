#include <iostream>
#include <string>

// #include "car.h"

using namespace std;

#include "speaker.h"

void Speaker::set_current_use(Speaker_Use *use) {
  this->current_use = use;

  use_func_type use_func = use->get_use_func();
  string use_name = use->get_name();
  use_func(use_name);
  thread tmp_thread = (thread(use_func, use_name));
  this->current_use_thread = &tmp_thread;
  this->current_use_thread->join();
}

void Speaker::unset_current_use(Speaker_Use *use) {
  kill_func_type kill_func = use->get_kill_func();
  kill_func();
}

bool Speaker::add_use(Speaker_Use *use) {
  // cout << "Add use++ " << use->get_name() << endl;
  if (!this->locked) {
    // no locked is on

    // only close locked when critical priority
    this->locked = use->get_priority() > 0 ? true : false;

    this->set_current_use(use);

    this->locked = false;
    return true;
  } else {
    // locked is on

    if (use->get_priority() > 0) {
      // critical use
      cout << "both critical" << '\n';
      if (this->current_use->get_name() == use->get_name()) {
        // we are trying to give the same warning
        return false;
      } else if (use->get_priority() > this->current_use->get_priority()) {
        // we have higher priority, so we will unlock

        // kill all other usage
        this->unset_current_use(use);

        this->locked = false;
      } else {
        // wait for the other to finish
        // cout << "waiting for the other one" << endl;
        sleep(1);
        return add_use(use);
      }
    }
  }
  return false;
}