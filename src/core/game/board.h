// Created by Chuilian Kong April 30, 2018

#ifndef CORE_GAME_BOARD_H_
#define CORE_GAME_BOARD_H_

#include <iostream>
#include <list>
#include <utility>
#include <cstring>
#include "utils/utils.h"
#include "core/game/ship.h"
#include "core/game/game_common.h"
#include "core/exception/exception.h"

class Board{
public:
  Board(){
    std::memset(states_, 0, sizeof(char) * kDim * kDim);
    std::memset(which_ship_, 0, sizeof(Ship*) * kDim * kDim);
  }

  // place a ship
  bool PlaceAShip(ShipType type, std::size_t head_location, Direction direction){
    if(!CanPlaceMore(type) || !DoesShipFit(type, head_location, direction)){
      return false;
    }

    // make a ship, append it to the ship list
    on_board_ships_.push_back(Ship(type));
    // ref to the new ship
    Ship& new_ship = on_board_ships_.back();

    // place the ship
    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // since we checked
        assert(row + size - 1 < kDim);
        // turn on OCCUPIED flag, connect the occupied location and the corresponding ship via pointer
        for(std::size_t i = 0; i < size; i++){
          states_[head_location + i * kDim] |= OCCUPIED;
          which_ship_[head_location + i * kDim] = &new_ship;
        }
        break;
      }
      case kHorisontal:{
        // since we checked
        assert(col + size - 1 < kDim);
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
    AddOneOnBoard(type);
    ships_alive_ += 1;

    return true;
  }

  // attack a location
  // return: the location is OCCUPIED or not (i.e., attack succeed or not),
  //         if the attack succeed, ShipType tells if a ship sinks, kNotAShip
  //         means no ships sink due to the current attack.
  // note: this method throws an exception
  AttackResult Attack(std::size_t location){
    // we prohibit player to attack the same spot multiple times
    // it will throw a exception
    if(states_[location] & ATTACKED){
      throw GameException::kLocationAlreadyAttacked;
    }

    states_[location] |= ATTACKED;

    if(states_[location] & OCCUPIED){
      // get ref of the attacked ship
      Ship* p_attacked_ship = which_ship_[location];
      p_attacked_ship -> Damage();
      if(p_attacked_ship -> IsAlive()){
        return AttackResult(location, true, kNotAShip, false);
      }else{
        DestroyOneOnBoard(p_attacked_ship->GetType());
        if(!Lose()){
          return AttackResult(location, true, p_attacked_ship -> GetType(), false);
        }else{
          return AttackResult(location, true, p_attacked_ship -> GetType(), true);
        }
      }
    }else{
      return AttackResult(location, false, kNotAShip, false);
    }

  }

  void SetGameOver(){
    is_game_over_ = true;
  }

  void SetThisWinner(){
    is_winner_me_ = true;
  }

  void IncrementOneMove(){
    move_num_ += 1;
  }

  size_t GetNumMoves(){
    return move_num_;
  }

private:
  // friends
  friend class GameUi;
  friend class ShipPlacementUnit;

  bool is_game_over_ = false;
  bool is_winner_me_ = false;

  size_t move_num_ = 0;

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

  // increment the on board ship number of given type
  void AddOneOnBoard(ShipType type){
    switch(type){
      case kCarrier:{
        assert(carrier_num_ < kCarrierNum);
        carrier_num_ += 1;
        break;
      }
      case kBattleShip:{
        assert(battleship_num_ < kBattleShipNum);
        battleship_num_+= 1;
        break;
      }
      case kCruiser:{
        assert(cruiser_num_ < kCruiserNum);
        cruiser_num_ += 1;
        break;
      }
      case kDestroyer:{
        assert(destroyer_num_ < kDestroyerNum);
        destroyer_num_ += 1;
        break;
      }
      default:{
        assert(false);
      }
    }
  }

  // TODO: possible dup
  // decrement the on board ship number of given type
  void DestroyOneOnBoard(ShipType type){
    switch(type){
      case kCarrier:{
        assert(carrier_num_ > 0);
        carrier_num_ -= 1;
        break;
      }
      case kBattleShip:{
        assert(battleship_num_ > 0);
        battleship_num_-= 1;
        break;
      }
      case kCruiser:{
        assert(cruiser_num_ > 0);
        cruiser_num_ -= 1;
        break;
      }
      case kDestroyer:{
        assert(destroyer_num_ > 0);
        destroyer_num_ -= 1;
        break;
      }
      case kNotAShip:{
        break;
      }
      default:{
        assert(false);
      }
    }
  }

  // check if I lose
  bool Lose(){
    return carrier_num_ == 0 && battleship_num_ == 0 && cruiser_num_ == 0 && destroyer_num_ == 0;
  }

};



#endif  // CORE_GAME_BOARD_H_
