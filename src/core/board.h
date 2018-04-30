// Created by Chuilian Kong April 30, 2018

#ifndef CORE_BOARD_H_
#define CORE_BOARD_H_

#include <iostream>
#include <list>
#include <cstring>
#include "../utils/utils.h"
#include "ship.h"

class Board{
public:
  Board(){
    std::memset(states_, 0, sizeof(char) * kDim * kDim);
    std::memset(which_ship_, 0, sizeof(Ship*) * kDim * kDim);
  }

private:
  static const std::size_t kDim = 10;
  static const std::size_t kCarrierNum = 1;
  static const std::size_t kBattleShipNum = 2;
  static const std::size_t kCruiserNum = 3;
  static const std::size_t kDestroyerNum = 4;

  // ships number currently on board
  std::size_t carrier_num_ = 0;
  std::size_t battleship_num_ = 0;
  std::size_t cruiser_num_ = 0;
  std::size_t destroyer_num_ = 0;

  // two bit flags
  static const unsigned char OCCUPIED = 1 << 0;
  static const unsigned char ATTACKED = 1 << 1;

  // ships that not on board, should be placed by calling PlaceShip
  //std::list<Ship> availale_ships_;
  // ships that on the board
  std::list<Ship> on_board_ships_;
  // live ships on the board
  std::size_t ships_alive_;
  // bit flag indicates the state of one spot
  unsigned char states_[kDim * kDim];
  // ship pointers indicates the ship pointer of one spot
  Ship* which_ship_[kDim * kDim];

  /*
  void MakeShips(ShipType type, std::size_t num){
    for(std::size_t i = 0; i < num; i++){
      availale_ships_.push_back(Ship(type));
    }
    ships_alive_ += num;
  }
  */

  // test if the given type ship fits the given place
  bool DoesShipFit(ShipType type, std::size_t head_location, Direction direction){
    if(head_location >= kDim * kDim) return false;

    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // boundary check
        if(row + size - 1 >= kDim) return false;
        // every subsequential spot should not be occupied
        for(std::size_t i = 0; i < size; i++){
          if(states_[head_location + i * kDim] & OCCUPIED) return false;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        if(col + size - 1 >= kDim) return false;
        // every subsequential spot should not be occupied
        for(std::size_t i = 0; i < size; i++){
          if(states_[head_location + i] & OCCUPIED) return false;
        }
        break;
      }
      default:{
        assert(false);
      }
    }

    return true;
  }

  // TODO: place a ship
  bool PlaceOneShip(ShipType ship, std::size_t head_location, Direction direction){
    //if(carrier_num_ >= kCarrierNum)

    return true;
  }

};



#endif  // CORE_BOARD_H_
