// Created by Chuilian Kong April 30, 2018

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <cassert>

// uncomment to disable assert()
// #define NDEBUG

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
  kDestoryer
};

#endif  // UTILS_UTILS_H_
