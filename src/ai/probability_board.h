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
    InitProbability();
  }

  size_t GetProbability(size_t location){
    return probability_board_[location];
  }

  size_t GetOneHighestProbabilityLocation(){
    size_t size = highest_probability_locations_.size();
    return highest_probability_locations_[RandomUnit::GetRandomSizeT(0, size - 1)];
  }

  void InitProbability(){
    // TODO: this can be simplified by hard coding probabilities of a blank enemy board
    RecalculateProbability();
  }

  // recalculate probability of the entire board
  void RecalculateProbability(){
    std::memset(probability_board_, 0, sizeof(size_t) * kDim * kDim);
    // TODO: iterate through directions can be further simplified
    std::vector<ShipType> types = GetShipTypeList();
    for(ShipType type : types){
      if(ref_enemy_board_.GetAliveShipNumber(type) > 0){
        for(size_t i = 0; i < kDim * kDim; ++i){
          if(ref_enemy_board_.DoesShipFit(type, i, Direction::kVertical)){
            IncrementProbablity(type, i, Direction::kVertical, ref_enemy_board_.GetAliveShipNumber(type));
          }
          if(ref_enemy_board_.DoesShipFit(type, i, Direction::kHorisontal)){
            IncrementProbablity(type, i, Direction::kHorisontal, ref_enemy_board_.GetAliveShipNumber(type));
          }
        }
      }
    }

    RemoveProbabilityAttackedLocations();
    UpdateStats();
    for(auto loca : highest_probability_locations_){
      Logger(std::to_string(loca) + "," + std::to_string(highest_probability_));
    }

  }

  // Update probabilities caused by changing of one location
  void UpdateProbabilityByLastAttackLocation(){
    Logger("last attacked: " + std::to_string(ref_enemy_board_.last_attack_location_));
    Logger("last attacked: " + std::to_string(ref_enemy_board_.last_attack_success_));
    Logger("last attacked: " + std::to_string(ref_enemy_board_.last_attack_sink_ship_type_));

    size_t location = ref_enemy_board_.last_attack_location_;
    unsigned char & ref_state = ref_enemy_board_.states_[location];
    // we update probabilities surround the last attack position
    // iff the last movement is not success
    if(!ref_enemy_board_.last_attack_success_){
      std::vector<ShipPlacementInfo> placements = GetImpactedPlacement(location);

      //Logger("last attacked: " + std::to_string(location));
      Logger("impacted placements:");


      for(auto placement : placements){
        Logger("type: " + std::to_string(placement.type) + ", location: " + std::to_string(placement.head_location) + ", direction: " + std::to_string(placement.direction));

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

    // we update probabilities of whole board,
    // iff the last movement is success and we sink a ship
    ShipType last_attack_sink_type = ref_enemy_board_.last_attack_sink_ship_type_;
    if(ref_enemy_board_.last_attack_success_ && last_attack_sink_type != ShipType::kNotAShip){
      // TODO: need a better way to update here
      RecalculateProbability();
      /*
      for(size_t i = 0; i < kDim * kDim; ++i){
        if(ref_enemy_board_.DoesShipFit(last_attack_sink_type, i, Direction::kVertical)){
          DecrementProbablity(last_attack_sink_type, i, Direction::kVertical, 1);
        }
        if(ref_enemy_board_.DoesShipFit(last_attack_sink_type, i, Direction::kHorisontal)){
          DecrementProbablity(last_attack_sink_type, i, Direction::kHorisontal, 1);
        }
      }
       */
    }

    // TODO: need a better way to remove probability for one movement
    //RemoveProbabilityLastAttackedLocation()
    RemoveProbabilityAttackedLocations();
    UpdateStats();

    for(auto loca : highest_probability_locations_){
      Logger(std::to_string(loca) + "," + std::to_string(highest_probability_));
    }

  }

  void RemoveProbabilityAttackedLocations(){
    for(size_t i = 0; i < kDim * kDim; ++i){
      if(ref_enemy_board_.states_[i] & ImagineBoard::ATTACKED){
        probability_board_[i] = 0;
      }
    }
  }

  void RemoveProbabilityLastAttackedLocation(){
    probability_board_[ref_enemy_board_.last_attack_location_] = 0;
  }


private:
  ImagineBoard& ref_enemy_board_;
  size_t probability_board_[kDim * kDim];

  // prob board stats
  size_t highest_probability_ = 0;
  size_t lowest_probability_ = 0;
  std::vector<size_t> highest_probability_locations_;


  void IncrementProbablity(ShipType type, size_t head_location, Direction direction, size_t increment){
    assert(head_location < kDim * kDim);

    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // boundary check
        assert(row + size - 1 < kDim);
        // every subsequential spot should not be (attacked but not occupied)
        // every subsequential spot should be (not attacked or (occupied))
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i * kDim] += increment;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        assert(col + size - 1 < kDim);
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
    assert(head_location < kDim * kDim);

    // TODO: possible code duplication
    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        // boundary check
        assert(row + size - 1 < kDim);
        for(std::size_t i = 0; i < size; i++){
          probability_board_[head_location + i * kDim] -= decrement;
        }
        break;
      }
      case kHorisontal:{
        // boundary check
        assert(col + size - 1 < kDim);
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
    lowest_probability_ = probability_board_[0];
    highest_probability_ = 0; // can't be probability_board_[0]
    highest_probability_locations_.clear();

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
    // TODO: iterate through directions can be further simplified

    std::vector<ShipType> types = GetShipTypeList();
    for(ShipType type : types){
      for(size_t i = 0; i < GetSizeFromType(type); ++i){
        size_t candidate = location - i;
        if(!OutOfBound(type,candidate,Direction::kHorisontal)
           && IsSameLine(candidate, location, Direction::kHorisontal)){
          res.emplace_back(ShipPlacementInfo(type, candidate, Direction::kHorisontal));
        }

        candidate = location - i * kDim;
        if(!OutOfBound(type,candidate,Direction::kVertical)
           && IsSameLine(candidate, location, Direction::kVertical)){
          res.emplace_back(ShipPlacementInfo(type, candidate, Direction::kVertical));
        }
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
    if(head_location >= kDim * kDim) return true;

    std::size_t size = GetSizeFromType(type);
    std::size_t row = head_location / kDim;
    std::size_t col = head_location % kDim;

    switch(direction){
      case kVertical:{
        if(row + size - 1 < kDim) return false;
        else return true;
      }
      case kHorisontal:{
        if(col + size - 1 < kDim) return false;
        else return true;
      }
      default:{
        assert(false);
      }
    }
  }

};

#endif //BATTLESHIP_GAME_PROBABILITY_BOARD_H
