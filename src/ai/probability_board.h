//
// Created by Kong, Chuilian on 5/23/18.
//

#ifndef BATTLESHIP_GAME_PROBABILITY_BOARD_H
#define BATTLESHIP_GAME_PROBABILITY_BOARD_H

#include <vector>
#include "core/game/game_common.h"
#include "core/game/imagine_board.h"
#include "ai/ai_common.h"

class ProbabilityBoard{
public:
  ProbabilityBoard(ImagineBoard& ref_enemy_board):
    ref_enemy_board_(ref_enemy_board){
    std::memset(probability_board_, 0, sizeof(size_t) * kDim * kDim);
  }

  // recalculate probability of the entire board
  void RecalculateProbability(){
    // TODO: possible code dup --> can be reduced by function GetAliveShipNumber! Please do.
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
    unsigned char & ref_state = ref_enemy_board_.states_[location];
    // we update probabilities surround the given position
    // iff the given position is marked ATTACKED but not OCCUPIED in the last movement
    if((ref_state & ImagineBoard::ATTACKED) && !(ref_state & ImagineBoard::OCCUPIED)){
      std::vector<ShipPlacementInfo> placements = GetImpactedPlacement(location);
      for(auto placement : placements){
        //TODO: this direct mofication of enemy board has potential HUGE multi-threading issue
        // turn ATTACKED off
        ref_state &= ~ImagineBoard::ATTACKED;
        bool old_fit = ref_enemy_board_.DoesShipFit(placement.type, placement.head_location, placement.direction);
        // turn ATTACKED back on
        ref_state |= ImagineBoard::ATTACKED;
        bool new_fit = ref_enemy_board_.DoesShipFit(placement.type, placement.head_location, placement.direction);
        if(old_fit && !new_fit){
          DecrementProbablity(placement.type, placement.head_location, placement.direction, ref_enemy_board_.GetAliveShipNumber(placement.type));
        }
      }
    }
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

  void DecrementProbablity(ShipType type, size_t head_location, Direction direction, size_t decrement){
    assert(head_location >= kDim * kDim);

    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // boundary check
        assert(row + size - 1 >= kDim);
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i * kDim] -= decrement;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        assert(col + size - 1 >= kDim);
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i] -= decrement;
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

  std::vector<ShipPlacementInfo> GetImpactedPlacement(size_t location){
    std::vector<ShipPlacementInfo> res;
    // TODO: possible code dup

    // carrier, size = 5
    for(size_t i = 0; i < 5; ++i){
      size_t candidate = location - i;
      if(!OutOfBound(ShipType::kCarrier,candidate,Direction::kHorisontal)
         && IsSameLine(candidate, location, Direction::kHorisontal)){
        res.emplace_back(ShipPlacementInfo(ShipType::kCarrier, candidate, Direction::kHorisontal));
      }

      candidate = location - i * kDim;
      if(!OutOfBound(ShipType::kCarrier,candidate,Direction::kVertical)
         && IsSameLine(candidate, location, Direction::kVertical)){
        res.emplace_back(ShipPlacementInfo(ShipType::kCarrier, candidate, Direction::kVertical));
      }
    }

    // kBattleShip, size = 4
    for(size_t i = 0; i < 4; ++i){
      size_t candidate = location - i;
      if(!OutOfBound(ShipType::kBattleShip,candidate,Direction::kHorisontal)
         && IsSameLine(candidate, location, Direction::kHorisontal)){
        res.emplace_back(ShipPlacementInfo(ShipType::kBattleShip, candidate, Direction::kHorisontal));
      }

      candidate = location - i * kDim;
      if(!OutOfBound(ShipType::kBattleShip,candidate,Direction::kVertical)
         && IsSameLine(candidate, location, Direction::kVertical)){
        res.emplace_back(ShipPlacementInfo(ShipType::kBattleShip, candidate, Direction::kVertical));
      }
    }

    // kCruiser, size = 3
    for(size_t i = 0; i < 3; ++i){
      size_t candidate = location - i;
      if(!OutOfBound(ShipType::kCruiser,candidate,Direction::kHorisontal)
         && IsSameLine(candidate, location, Direction::kHorisontal)){
        res.emplace_back(ShipPlacementInfo(ShipType::kCruiser, candidate, Direction::kHorisontal));
      }

      candidate = location - i * kDim;
      if(!OutOfBound(ShipType::kCruiser,candidate,Direction::kVertical)
         && IsSameLine(candidate, location, Direction::kVertical)){
        res.emplace_back(ShipPlacementInfo(ShipType::kCruiser, candidate, Direction::kVertical));
      }
    }

    // kDestroyer, size = 2
    for(size_t i = 0; i < 2; ++i){
      size_t candidate = location - i;
      if(!OutOfBound(ShipType::kDestroyer,candidate,Direction::kHorisontal)
         && IsSameLine(candidate, location, Direction::kHorisontal)){
        res.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, candidate, Direction::kHorisontal));
      }

      candidate = location - i * kDim;
      if(!OutOfBound(ShipType::kDestroyer,candidate,Direction::kVertical)
         && IsSameLine(candidate, location, Direction::kVertical)){
        res.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, candidate, Direction::kVertical));
      }
    }

    return res;
  }

  bool IsSameLine(size_t head_location_a, size_t head_location_b, Direction direction){
    std::size_t row_a = head_location_a / kDim;
    std::size_t col_a = head_location_a % kDim;
    std::size_t row_b = head_location_b / kDim;
    std::size_t col_b = head_location_b % kDim;
    switch(direction){
      case kVertical:{
        return col_a == col_b;
      }
      case kHorisontal:{
        return row_a == row_b;
      }
      default:{
        assert(false);
      }
    }
  }


  // TODO: this boundary check function can be applied to game board, and imagine_board etc.
  bool OutOfBound(ShipType type, size_t head_location, Direction direction){
    if(head_location < 0 || head_location >= kDim * kDim) return false;

    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // since we checked
        if(row + size - 1 < kDim) return true;
        break;
      }
      case kHorisontal:{
        // since we checked
        if(col + size - 1 < kDim) return true;
        break;
      }
      default:{
        assert(false);
      }
    }

  }

};

#endif //BATTLESHIP_GAME_PROBABILITY_BOARD_H
