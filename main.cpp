#include <csignal>
#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

#define MYCAR gmc

using namespace std;

#include "car.h"

void disable_terminal() {
  struct termios info;

  /* get current terminal attirbutes; 0 is the file descriptor for stdin */
  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON; /* disable canonical mode */
  info.c_cc[VMIN] = 1;     /* wait until at least one keystroke available */
  info.c_cc[VTIME] = 0;    /* no timeout */
  tcsetattr(0, TCSANOW, &info); /* set immediately */
}

void static_move_car(Car *car) { car->move_car(); }
void static_write_car(Car *car) {
  while (true) {
    car->write_car(true);
  }
}

void driving_input(Car *car) {
  disable_terminal();

  thread move_thread(static_move_car, car);
  thread write_thread(static_write_car, car);

  char input;
  while ((input = getchar()) != 27 /* ascii ESC */) {
    if (input < 0) {
      if (ferror(stdin)) { /* there was an error... */
      }
      clearerr(stdin);
      /* do other stuff */
    } else {
      // cout << "char: " << input << '\n';
      // cout << ' ';

      switch (input) {
      case 's':
        if (car->get_on()) {
          car->turnoff();
        } else {
          car->start();
        }
        break;

      case 'p':
        car->change_transmission('p');
        break;
      case 'n':
        car->change_transmission('n');
        break;
      case 'd':
        car->change_transmission('d');
        break;
      case 'r':
        car->change_transmission('r');
        break;
      case 'c':
        car->toggle_cruise_control();
        break;
      case ' ':
        car->brake(1);
        break;

      default:
        if ('1' <= input && input <= '9') {
          car->give_gas((double)(input - '0') / 10.0);
        }
        if (input == '0') {
          car->give_gas(1);
        }
      }
    }
  }

  move_thread.detach();
  write_thread.detach();
}

void start_car(Car *car) { car->start(); }

void see_on(Car *car) { cout << car->start() << '\n'; }

int main() {
  Car *car = new Car("Yukon", 2020);
  cout << "Car created" << '\n';

  thread driving_thread(driving_input, car);
  // thread writing_thread(write_driving_parameters, car);

  // car->start();

  driving_thread.join();
  // writing_thread.join();

  return 0;
}
