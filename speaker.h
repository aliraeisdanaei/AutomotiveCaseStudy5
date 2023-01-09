#include <iostream>
#include <string>
#include <thread>
#include <list>

#include "car.h"

using namespace std;

#ifndef SPEAKER

class Speaker_Use {
private:
    bool critical_priority;
    string name;
    void(*use_func)(string);

public:
    Speaker_Use(bool critical_priority, string name, void(*use_func)(string)){
        this->critical_priority = critical_priority;
        this->name = name;
        this->use_func = use_func;
    }

    ~Speaker_Use(){
        delete &name;
    }

    bool get_critical_priority(){ return this->critical_priority; }
    string get_name(){ return this->name; }

  static void give_speaker_warning(string warning_msg) {
    string espeak_cmd = "espeak \'warning " + warning_msg + '\'';
    system(espeak_cmd.c_str());
  }

};

class Speaker {
private:
    Speaker_Use *current_use;
public:
    bool add_use(Speaker_Use* use){
        if(use->get_priority )
    }
};

#define SPEAKER
#endif