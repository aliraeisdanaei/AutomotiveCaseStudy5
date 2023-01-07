#include <cstddef>
#include <iostream>
#include <string>

#define MYCAR gmc

using namespace std;

#include "car.h"

int main() {
  Car car("Yukon", 2020);
  string input;

  while (input != "exit") {
    cout << "Enter input command: ";
    cin >> input;

#ifdef MYCAR
    if (input == "start") {
      if (car.start()) {
        cout << "Car started\n";
      }
    }

    if (input == "turnoff") {
      if (!car.turnoff()) {
        cout << "Car turned off\n";
      }
    }
#endif
  }
  return 0;
}
