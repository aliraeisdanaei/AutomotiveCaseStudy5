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

bool Speaker::add_use(Speaker_Use *use) {
  cout << "Add use++ " << use->get_name() << endl;
  if (!this->locked) {
    // no locked is on

    // only close locked when critical priority
    this->locked = use->get_critical_priority() ? true : false;
    // TODO kill all other usages
    this->set_current_use(use);
    this->locked = false;
    return true;
  } else {
    // locked is on

    if (use->get_critical_priority()) {
      // critical use
      cout << "both critical" << '\n';
      if (this->current_use->get_name() == use->get_name()) {
        // we are trying to give the same warning
        // cout << "Not allowed ";
        return false;
      } else {
        // wait for the other to finish
        cout << "waiting for the other one" << endl;
        sleep(1);
        return add_use(use);
      }
    }
  }
  return false;
}