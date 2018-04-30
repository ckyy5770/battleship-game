// Created by Chuilian Kong April 30, 2018

#ifndef CORE_BOARD_H_
#define CORE_BOARD_H_

#include <iostream>
#include <vector>
#include <cstring>
#include "../utils/utils.h"
#include "ship.h"

class Board{
public:
  Board(){
    std::memset(states_, 0, sizeof(char) * kDim * kDim);

    MakeShips(kCarrier, kCarrierNum);
    MakeShips(kBattleShip, kBattleShipNum);
    MakeShips(kCruiser, kCruiserNum);
    MakeShips(kDestoryer, kDestoryerNum);
  }
private:
  static const std::size_t kDim = 10;
  static const std::size_t kCarrierNum = 1;
  static const std::size_t kBattleShipNum = 2;
  static const std::size_t kCruiserNum = 3;
  static const std::size_t kDestoryerNum = 4;
  std::vector<Ship> ships_;
  std::size_t ships_alive_;
  char states_[kDim * kDim];

  void MakeShips(ShipType type, std::size_t num){
    for(int i = 0; i < num; i++){
      ships_.push_back(Ship(type));
    }
    ships_alive_ += num;
  }
};

#endif  // CORE_BOARD_H_
