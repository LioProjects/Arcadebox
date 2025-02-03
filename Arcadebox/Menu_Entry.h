#ifndef Menu_Entry_H
#define Menu_Entry_H
#include "Game.h"

struct Coordinate {
  byte row;
  byte col;
};

struct MenuEntry {
  static byte nextAvailableId;

  byte gameId;
  Game* game;
  String gameName;
  Coordinate coordinates;

  MenuEntry(Game *game);

  private:
  Coordinate calculateCoordinateOnMenu(byte position, byte nameLength);
};

#endif