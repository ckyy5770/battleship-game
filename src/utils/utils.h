// Created by Chuilian Kong April 30, 2018

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <iostream>
#include <string>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG

typedef unsigned int ClientId;
typedef unsigned int GameId;

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

enum Exception{
  kLocationAlreadyAttacked
};

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

static void Logger(const std::string & what){
  std::cout << what << std::endl;
}

#endif  // UTILS_UTILS_H_
