#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

// #include "../car.h"
// #include "../speaker.h"

using namespace std;

#include "../configuration.h"
#ifndef MUSIC_PLAYER
#define MUSIC_PLAYER
#endif

class Music_Player {
private:
public:
  string song1 = "Music_Player/ACDC_HighwayToHell.mp3";

  static void play_mpv(string mp3_filename) {
    // string play_mpv_cmd = "mpv --really-quiet \'" + mp3_filename + "\' ";
    // string play_mpv_cmd = "mpv --no-terminal --really-quiet \'" +
    // mp3_filename + "\' > tmp.o";
    string play_mpv_cmd = "mpv --really-quiet \'" + mp3_filename + "\'";
    system(play_mpv_cmd.c_str());
  }

  static void kill_mpv() {
    // string kill_mpv_cmd = "kill -s STOP $(pidof mpv)";
    string kill_mpv_cmd = "killall mpv";
    system(kill_mpv_cmd.c_str());
  }
};