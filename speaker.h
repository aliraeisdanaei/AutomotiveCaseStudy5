#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <unistd.h>

using namespace std;

#ifndef SPEAKER

typedef void (*use_func_type)(string);

class Speaker_Use {
private:
  bool critical_priority;
  string name;
  use_func_type use_func;

public:
  Speaker_Use(bool critical_priority, string name, use_func_type use_func) {
    this->critical_priority = critical_priority;
    this->name = name;
    this->use_func = use_func;
  }

  ~Speaker_Use() { delete &name; }

  bool get_critical_priority() { return this->critical_priority; }
  string get_name() { return this->name; }
  use_func_type get_use_func() { return this->use_func; }

  static void give_speaker_warning(string warning_msg) {
    string espeak_cmd = "espeak \'warning " + warning_msg + '\'';
    system(espeak_cmd.c_str());
  }
};

class Speaker {
private:
  // true iff locked
  bool lock;
  Speaker_Use *current_use = (Speaker_Use *)malloc(sizeof(Speaker_Use));
  thread *current_use_thread = (thread *)malloc(sizeof(thread));

public:
  Speaker() { this->lock = false; }

  ~Speaker() {
    delete this->current_use;
    delete this->current_use_thread;
  }

  void set_current_use(Speaker_Use *use) {
    this->current_use = use;

    use_func_type use_func = use->get_use_func();
    string use_name = use->get_name();
    thread tmp_thread = (thread(use_func, use_name));
    this->current_use_thread = &tmp_thread;
    this->current_use_thread->join();
  }

  bool add_use(Speaker_Use *use) {
    cout << "Add use++ " << use->get_name() << endl;
    if (!this->lock) {
      // no lock is on

      // only close lock when critical priority
      this->lock = use->get_critical_priority() ? true : false;
      this->set_current_use(use);
      this->lock = false;
      return true;
    } else {
      // lock is on

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
};

#define SPEAKER
#endif