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
  bool empty;
  Speaker_Use *current_use = (Speaker_Use *)malloc(sizeof(Speaker_Use));
  thread *current_use_thread = (thread *)malloc(sizeof(thread));

public:
  Speaker() { this->empty = true; }

  ~Speaker() {
    delete this->current_use;
    delete this->current_use_thread;
  }

  bool add_use(Speaker_Use *use) {
    cout << "Add use " << use->get_name() << '\n';
    if (this->empty) {
      use_func_type use_func = use->get_use_func();
      string use_name = use->get_name();
      this->empty = false;
      thread tmp_thread = (thread(use_func, use_name));
      this->current_use_thread = &tmp_thread;
      this->current_use_thread->join();
      this->empty = true;
      return true;
    } else {
      if (use->get_critical_priority()) {
        // critical use
        if (this->current_use->get_critical_priority()) {
          if (this->current_use->get_name() == use->get_name()) {
            // we are trying to give the same warning
            sleep(5);
            return false;
          } else {
            // wait for the other to finish
            this->current_use_thread->join();

            use_func_type use_func = use->get_use_func();
            string use_name = use->get_name();
            this->empty = false;
            thread tmp_thread = (thread(use_func, use_name));
            this->current_use_thread = &tmp_thread;
            this->current_use_thread->join();
            this->empty = true;
            return true;
          }
        }
      } else {
        // non-critical use
        if (current_use->get_critical_priority()) {
          // cannot be played
          return false;
        } else {
          // replaces the other use
          this->current_use_thread->detach();

          use_func_type use_func = use->get_use_func();
          string use_name = use->get_name();
          this->empty = false;
          thread tmp_thread = (thread(use_func, use_name));
          this->current_use_thread = &tmp_thread;
          this->current_use_thread->join();
          this->empty = true;
          return true;
        }
      }
    }
    return false;
  }
};

#define SPEAKER
#endif