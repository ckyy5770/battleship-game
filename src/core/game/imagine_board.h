// Created by Chuilian Kong April 30, 2018

#ifndef CORE_GAME_IMAGINE_BOARD_H_
#define CORE_GAME_IMAGINE_BOARD_H_
#include <iostream>
#include <list>
#include <utility>
#include <cstring>
#include "utils/utils.h"
#include "ship.h"

class ImagineBoard{
public:
  ImagineBoard(){
    std::memset(states_, 0, sizeof(char) * kDim * kDim);
  }

  void MarkAttack(std::size_t location){
    states_[location] |= ATTACKED;
  }

  void MarkOccupied(std::size_t location){
    states_[location] |= OCCUPIED;
  }

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

  void SetGameOver(){
    is_game_over_ = true;
  }

  void SetThisWinner(){
    is_winner_me_ = true;
  }

  void IncrementOneMove(){
    move_num_ += 1;
  }

private:
  // friends
  friend class GameUi;

  bool is_game_over_ = false;
  bool is_winner_me_ = false;

  size_t move_num_ = 0;

  // ships number currently on board
  std::size_t carrier_num_ = 1;
  std::size_t battleship_num_ = 2;
  std::size_t cruiser_num_ = 3;
  std::size_t destroyer_num_ = 4;

  // two bit flags
  static const unsigned char OCCUPIED = 1 << 0;
  static const unsigned char ATTACKED = 1 << 1;

  // bit flag indicates the state of one spot
  unsigned char states_[kDim * kDim];

};


#endif  // CORE_GAME_IMAGINE_BOARD_H_
