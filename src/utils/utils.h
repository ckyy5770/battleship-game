// Created by Chuilian Kong April 30, 2018

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <iostream>
#include <string>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG

static const std::size_t kDim = 10;
static const std::size_t kCarrierNum = 1;
static const std::size_t kBattleShipNum = 2;
static const std::size_t kCruiserNum = 3;
static const std::size_t kDestroyerNum = 4;

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

enum GameException{
  kLocationAlreadyAttacked,
  kGameOverWin,
  kGameOverLose
};

struct AttackResult{
  size_t location;
  bool success;
  ShipType sink_ship_type;
  bool attacker_win;

  AttackResult(size_t location, bool success, ShipType sink_ship_type, bool attacker_win):
    location(location),
    success(success),
    sink_ship_type(sink_ship_type),
    attacker_win(attacker_win){};
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
