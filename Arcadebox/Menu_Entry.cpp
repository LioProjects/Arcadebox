#include "Menu_Entry.h"

byte MenuEntry::nextAvailableId = 0;

MenuEntry::MenuEntry(Game* game){
  this->gameId = nextAvailableId++;
  this->game = game;
  this->gameName = game->getName();
  this->coordinates = calculateCoordinateOnMenu(this->gameId, this->gameName.length());
}

Coordinate MenuEntry::calculateCoordinateOnMenu(byte position, byte nameLenght){
  Coordinate coordinate;
  coordinate.row = position / 2 + 2;
  coordinate.col = (position % 2) * 10 + (10 - nameLenght) / 2;
  return coordinate;
}
