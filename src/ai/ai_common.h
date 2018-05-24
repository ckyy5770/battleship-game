//
// Created by Kong, Chuilian on 5/23/18.
//

#ifndef BATTLESHIP_GAME_AI_COMMON_H
#define BATTLESHIP_GAME_AI_COMMON_H

#include "core/game/ship.h"

struct ShipPlacementInfo{
  ShipType type;
  std::size_t head_location;
  Direction direction;

  ShipPlacementInfo(){};
  ShipPlacementInfo(ShipType type, std::size_t head_location, Direction direction):
    type(type),
    head_location(head_location),
    direction(direction){};
};
#endif //BATTLESHIP_GAME_AI_COMMON_H
