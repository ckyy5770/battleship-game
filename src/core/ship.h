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
        size_ = 5;
        remaining_life_ = 5;
        break;
      }
      case kBattleShip:{
        size_ = 4;
        remaining_life_ = 4;
        break;
      }
      case kCruiser:{
        size_ = 3;
        remaining_life_ = 3;
        break;
      }
      case kDestoryer:{
        size_ = 2;
        remaining_life_ = 2;
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
  std::size_t size_;
  std::size_t remaining_life_;
  std::size_t head_location_;
  Direction direction_;
};

#endif  // CORE_SHIP_H_
