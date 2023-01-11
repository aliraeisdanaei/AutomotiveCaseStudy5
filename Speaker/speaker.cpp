#include <iostream>
#include <string>

// #include "car.h"

using namespace std;

#include "speaker.h"

void Speaker::set_current_use(Speaker_Use *use) {
  this->current_use = use;

  use_func_type use_func = use->get_use_func();
  string use_name = use->get_name();
  // use_func(use_name);
  thread tmp_thread = (thread(use_func, use_name));
  this->current_use_thread = &tmp_thread;
  this->current_use_thread->join();
}

void Speaker::unset_current_use() {
  kill_func_type kill_func = this->current_use->get_kill_func();
  kill_func();
  this->locked = false;
}

bool Speaker::add_use(Speaker_Use *use) {
  cout << "Add use++ " << use->get_name() << endl;
  if (!this->locked) { // no locked is on
    // this->locked = use->get_priority() > 0 ? true : false;
    this->locked = true;
    cout << "Locked" << endl;
    this->set_current_use(use);
    cout << "unLocked" << endl;
    this->locked = false;
    return true;
  } else { // locked is on
    if (use->get_priority() > 0) {
      // critical use
      cout << "critical" << '\n';
      if (this->current_use->get_name() == use->get_name()) {
        // we are trying to give the same warning
        cout << "Same Warning" << endl;
        return false;
      } else if (use->get_priority() > this->current_use->get_priority()) {
        cout << "We have higher priority we will kill" << endl;
        this->unset_current_use();
        return add_use(use);
      } else {
        // wait for the other to finish
        cout << "waiting for the other one" << endl;
        sleep(1);
        return add_use(use);
      }
    } else if (this->current_use->get_priority() == 0 &&
               use->get_priority() == 0) {
      cout << "Both Non-critical" << endl;
      this->unset_current_use();
      return add_use(use);
    }
  }
  return false;
}