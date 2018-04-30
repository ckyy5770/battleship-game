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

  // place a ship
  bool PlaceOneShip(ShipType type, std::size_t head_location, Direction direction){
    if(!CanPlaceMore(type) || !DoesShipFit(type, head_location, direction)){
      return false;
    }

    // make a ship, append it to the ship list
    on_board_ships_.push_back(Ship(type));
    // ref to the new ship
    Ship new_ship = on_board_ships_.back();

    // place the ship
    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // since we checked
        assert(row + size - 1 >= kDim);
        // turn on OCCUPIED flag, connect the occupied location and the corresponding ship via pointer
        for(std::size_t i = 0; i < size; i++){
          states_[head_location + i * kDim] |= OCCUPIED;
          which_ship_[head_location + i * kDim] = &new_ship;
        }
        break;
      }
      case kHorisontal:{
        // since we checked
        assert(col + size - 1 >= kDim);
        // turn on OCCUPIED flag, connect the occupied location and the corresponding ship via pointer
        for(std::size_t i = 0; i < size; i++){
          states_[head_location + i] |= OCCUPIED;
          which_ship_[head_location + i] = &new_ship;
        }
        break;
      }
      default:{
        assert(false);
      }
    }

    new_ship.PlaceShip(head_location, direction);
    ships_alive_ += 1;

    return true;
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

  // ships that on the board
  std::list<Ship> on_board_ships_;
  // the number of live ships on the board
  std::size_t ships_alive_;
  // bit flag indicates the state of one spot
  unsigned char states_[kDim * kDim];
  // ship pointers indicates the ship pointer of one spot
  Ship* which_ship_[kDim * kDim];

  // test if the given type ship fits the given place
  bool DoesShipFit(ShipType type, std::size_t head_location, Direction direction){
    if(head_location >= kDim * kDim) return false;

    // TODO: possible code duplication
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

  // test if there are enough number of certain type of ships on board
  bool CanPlaceMore(ShipType type){
    switch(type){
      case kCarrier:{
        return carrier_num_ < kCarrierNum;
      }
      case kBattleShip:{
        return battleship_num_ < kBattleShipNum;
      }
      case kCruiser:{
        return cruiser_num_ < kCruiserNum;
      }
      case kDestroyer:{
        return destroyer_num_ < kDestroyerNum;
      }
      default:{
        assert(false);
      }
    }
  }




};



#endif  // CORE_BOARD_H_
