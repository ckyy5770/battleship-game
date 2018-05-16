#ifndef CORE_GAME_GAME_H_
#define CORE_GAME_GAME_H_

#include "core/game/board.h"
#include "core/game/imagine_board.h"
#include "core/game/ship.h"
#include "core/game/utils.h"


// define Directions
enum Direction{
  kVertical,
  kHorisontal,
  kUndefined
};

// define Ship types
enum ShipType{
  kCarrier,
  kBattleShip,
  kCruiser,
  kDestroyer,
  kNotAShip
};

#endif  // CORE_GAME_GAME_H_
