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

  void UpdateLastAttackInfo(const AttackResult & res){
    last_attack_location_ = res.location;
    last_attack_success_ = res.success;
    last_attack_sink_ship_type_ = res.sink_ship_type;
  }

  std::vector<size_t> GetUnAttackedLocations(){
    std::vector<size_t> res;
    for(size_t i = 0; i < kDim * kDim; ++i){
      if(!LocationAttacked(i)){
        res.emplace_back(i);
      }
    }
    return res;
  }

  std::vector<size_t> GetSurroundingFourUnAttacked(size_t location){
    std::vector<size_t> res;
    const size_t kDoesntExist = kDim * kDim;
    size_t left = location > 0 ? location - 1 : kDoesntExist;
    size_t right = location % kDim < kDim - 1 ? location + 1 : kDoesntExist;
    size_t up = location / kDim > 0 ? location - kDim : kDoesntExist;
    size_t down = location / kDim < kDim - 1 ? location + kDim : kDoesntExist;

    if(left != kDoesntExist && !LocationAttacked(left)) res.emplace_back(left);
    if(right != kDoesntExist && !LocationAttacked(right)) res.emplace_back(right);
    if(up != kDoesntExist && !LocationAttacked(up)) res.emplace_back(up);
    if(down != kDoesntExist && !LocationAttacked(down)) res.emplace_back(down);

    return res;
  }

  bool LocationAttacked(size_t location){
    return states_[location] & ATTACKED;
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
        // every subsequential spot should not be (attacked but not occupied)
        // every subsequential spot should be (not attacked or (occupied))
        for(std::size_t i = 0; i < size; i++){
          unsigned char state = states_[head_location + i * kDim];
          if((state & ATTACKED) && !(state & OCCUPIED)) return false;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        if(col + size - 1 >= kDim) return false;
        // every subsequential spot should not be (attacked but not occupied)
        // every subsequential spot should be (not attacked or (occupied))
        for(std::size_t i = 0; i < size; i++){
          unsigned char state = states_[head_location + i];
          if((state & ATTACKED) && !(state & OCCUPIED)) return false;
        }
        break;
      }
      default:{
        assert(false);
      }
    }

    return true;
  }

  // TODO: this can be used to reduce code dup in probability attack module
  size_t GetAliveShipNumber(ShipType type){
    switch(type){
      case ShipType::kCarrier:{
        return cruiser_num_;
      }
      case ShipType::kBattleShip:{
        return battleship_num_;
      }
      case ShipType::kCruiser:{
        return cruiser_num_;
      }
      case ShipType::kDestroyer:{
        return destroyer_num_;
      }
      default:{
        assert(false);
      }
    }
  }

private:
  // friends
  friend class GameUi;
  friend class AttackLocationUnit;
  friend class ProbabilityBoard;

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

  // my brian should remember some stuff
  size_t last_attack_location_;
  bool last_attack_success_;
  ShipType last_attack_sink_ship_type_;

};


#endif  // CORE_GAME_IMAGINE_BOARD_H_
