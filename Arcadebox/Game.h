#ifndef Game_h
#define Game_h
#include "Arduino.h" 

class Game {
public:
  virtual void runGame() = 0;
  String getName() const { return name; }

protected:
  String name;
};


#endif