//
// Created by Kong, Chuilian on 5/23/18.
//

#ifndef BATTLESHIP_GAME_PROBABILITY_BOARD_H
#define BATTLESHIP_GAME_PROBABILITY_BOARD_H

#include <vector>
#include "core/game/game_common.h"
#include "core/game/imagine_board.h"

class ProbabilityBoard{
public:
  ProbabilityBoard(ImagineBoard& ref_enemy_board):
    ref_enemy_board_(ref_enemy_board){
    std::memset(probability_board_, 0, sizeof(size_t) * kDim * kDim);
  }

  // recalculate probability of the entire board
  void RecalculateProbability(){
    // TODO: possible code dup
    if(ref_enemy_board_.carrier_num_ > 0){
      for(size_t i = 0; i < kDim * kDim; ++i){
        if(ref_enemy_board_.DoesShipFit(ShipType::kCarrier, i, Direction::kVertical)){
          IncrementProbablity(ShipType::kCarrier, i, Direction::kVertical, ref_enemy_board_.carrier_num_);
        }
        if(ref_enemy_board_.DoesShipFit(ShipType::kCarrier, i, Direction::kHorisontal)){
          IncrementProbablity(ShipType::kCarrier, i, Direction::kVertical, ref_enemy_board_.carrier_num_);
        }
      }
    }

    if(ref_enemy_board_.battleship_num_ > 0){
      for(size_t i = 0; i < kDim * kDim; ++i){
        if(ref_enemy_board_.DoesShipFit(ShipType::kBattleShip, i, Direction::kVertical)){
          IncrementProbablity(ShipType::kBattleShip, i, Direction::kVertical, ref_enemy_board_.battleship_num_);
        }
        if(ref_enemy_board_.DoesShipFit(ShipType::kBattleShip, i, Direction::kHorisontal)){
          IncrementProbablity(ShipType::kBattleShip, i, Direction::kVertical, ref_enemy_board_.battleship_num_);
        }
      }
    }

    if(ref_enemy_board_.cruiser_num_ > 0){
      for(size_t i = 0; i < kDim * kDim; ++i){
        if(ref_enemy_board_.DoesShipFit(ShipType::kCruiser, i, Direction::kVertical)){
          IncrementProbablity(ShipType::kCruiser, i, Direction::kVertical, ref_enemy_board_.cruiser_num_);
        }
        if(ref_enemy_board_.DoesShipFit(ShipType::kCruiser, i, Direction::kHorisontal)){
          IncrementProbablity(ShipType::kCruiser, i, Direction::kVertical, ref_enemy_board_.cruiser_num_);
        }
      }
    }

    if(ref_enemy_board_.destroyer_num_ > 0){
      for(size_t i = 0; i < kDim * kDim; ++i){
        if(ref_enemy_board_.DoesShipFit(ShipType::kDestroyer, i, Direction::kVertical)){
          IncrementProbablity(ShipType::kDestroyer, i, Direction::kVertical, ref_enemy_board_.destroyer_num_);
        }
        if(ref_enemy_board_.DoesShipFit(ShipType::kDestroyer, i, Direction::kHorisontal)){
          IncrementProbablity(ShipType::kDestroyer, i, Direction::kVertical, ref_enemy_board_.destroyer_num_);
        }
      }
    }

    UpdateStats();
  }

  // Update probabilities caused by changing of one location
  void UpdateProbability(size_t location){
    // TODO

  }


private:
  ImagineBoard& ref_enemy_board_;
  size_t probability_board_[kDim * kDim];

  // prob board stats
  size_t highest_probability_ = 0;
  size_t lowest_probability_ = 0;
  std::vector<size_t> highest_probability_locations_;


  void IncrementProbablity(ShipType type, size_t head_location, Direction direction, size_t increment){
    assert(head_location >= kDim * kDim);

    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // boundary check
        assert(row + size - 1 >= kDim);
        // every subsequential spot should not be (attacked but not occupied)
        // every subsequential spot should be (not attacked or (occupied))
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i * kDim] += increment;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        assert(col + size - 1 >= kDim);
        // every subsequential spot should not be (attacked but not occupied)
        // every subsequential spot should be (not attacked or (occupied))
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i] += increment;
        }
        break;
      }
      default:{
        assert(false);
      }
    }

  }

  void UpdateStats(){
    for(size_t i = 0; i < kDim * kDim; ++i){
      if(probability_board_[i] > highest_probability_){
        highest_probability_ = probability_board_[i];
        highest_probability_locations_.clear();
        highest_probability_locations_.emplace_back(i);
      }else if(probability_board_[i] == highest_probability_){
        highest_probability_locations_.emplace_back(i);
      }

      if(probability_board_[i] < lowest_probability_){
        lowest_probability_ = probability_board_[i];
      }
    }
  }

};

#endif //BATTLESHIP_GAME_PROBABILITY_BOARD_H
