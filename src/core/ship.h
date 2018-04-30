// Created by Chuilian Kong April 30, 2018

#ifndef CORE_SHIP_H_
#define CORE_SHIP_H_

#include <iostream>
#include "../utils/utils.h"

class Ship{
public:
  Ship(ShipType type)
    : type_(type),
      head_location_(0),
      direction_(kUndefined){
    switch(type_){
      case kCarrier:{
        remaining_life_ = GetSizeFromType(kCarrier);
        break;
      }
      case kBattleShip:{
        remaining_life_ = GetSizeFromType(kBattleShip);
        break;
      }
      case kCruiser:{
        remaining_life_ = GetSizeFromType(kCruiser);
        break;
      }
      case kDestroyer:{
        remaining_life_ = GetSizeFromType(kDestroyer);
        break;
      }
      default:{
        assert(false);
      }
    }
  }

  void PlaceShip(std::size_t head_location, Direction direction){
    head_location_ = head_location;
    direction_ = direction;
  }

  void Damage(){
    assert(remaining_life_ > 0);
    remaining_life_ -= 1;
  }

  bool IsAlive(){
    return remaining_life_ > 0;
  }

private:
  ShipType type_;
  std::size_t remaining_life_;
  std::size_t head_location_;
  Direction direction_;
};

#endif  // CORE_SHIP_H_
