#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <unistd.h>

#include "../configuration.h"

// #include "speaker.h"
// #include "../configuration.h"

using namespace std;

typedef void (*use_func_type)(string);
typedef void (*kill_func_type)();

class Speaker_Use {
private:
  bool critical_priority;
  string name;
  use_func_type use_func;
  kill_func_type kill_func;

public:
  Speaker_Use(bool critical_priority, string name, use_func_type use_func,
              kill_func_type kill_func) {
    this->critical_priority = critical_priority;
    this->name = name;
    this->use_func = use_func;
    this->kill_func = kill_func;
  }

  ~Speaker_Use() { delete &name; }

  bool get_critical_priority() { return this->critical_priority; }
  string get_name() { return this->name; }
  use_func_type get_use_func() { return this->use_func; }
  kill_func_type get_kill_func() { return this->kill_func; }

  static void give_speaker_warning(string warning_msg) {
    string espeak_cmd = "espeak \'warning " + warning_msg + '\'';
    system(espeak_cmd.c_str());
  }

  static void kill_speaker_warning() {
    string kill_espeak_cmd = "killall espeak";
    system(kill_espeak_cmd.c_str());
  }
};
