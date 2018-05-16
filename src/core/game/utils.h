// Created by Chuilian Kong May 16, 2018

#ifndef CORE_GAME_UTILS_H_
#define CORE_GAME_UTILS_H_

#include "core/game/game.h"

// get size of the ship
std::size_t GetSizeFromType(ShipType type){
  switch(type){
    case kCarrier:{
      return 5;
    }
    case kBattleShip:{
      return 4;
    }
    case kCruiser:{
      return 3;
    }
    case kDestroyer:{
      return 2;
    }
    default:{
      assert(false);
    }
  }
}

#endif  // CORE_GAME_UTILS_H_
