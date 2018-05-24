//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H
#define BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H

#include <vector>
#include <memory>
#include "ai/random_unit.h"
#include "ai/ai_common.h"
#include "core/game/board.h"


enum class StrategyPlaceShip{
  kFixed,
  kRandom
};

class ShipPlacementUnit{
public:
  ShipPlacementUnit(){}

  std::vector<ShipPlacementInfo> ShipPlacingPlan(const StrategyPlaceShip & strategy){
    switch(strategy){
      case StrategyPlaceShip::kFixed:{
        return ShipPlacingPlanFixed();
      }
      case StrategyPlaceShip::kRandom:{
        return ShipPlacingPlanRandom();
      }
      default:{
        assert(false);
      }
    }

  }
private:
  Board trial_board_;

  // ship placement strategies
  std::vector<ShipPlacementInfo> ShipPlacingPlanFixed(){
    std::vector<ShipPlacementInfo> ret;
    ret.emplace_back(ShipPlacementInfo(ShipType::kCarrier, 0, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kBattleShip, 1, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kBattleShip, 2, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kCruiser, 3, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kCruiser, 4, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kCruiser, 5, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, 6, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, 7, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, 8, Direction::kVertical));
    ret.emplace_back(ShipPlacementInfo(ShipType::kDestroyer, 9, Direction::kVertical));

    return ret;
  }

  std::vector<ShipPlacementInfo> ShipPlacingPlanRandom(){
    std::vector<ShipPlacementInfo> res;
    PlaceOneType(ShipType::kCarrier, res);
    PlaceOneType(ShipType::kBattleShip, res);
    PlaceOneType(ShipType::kCruiser, res);
    PlaceOneType(ShipType::kDestroyer, res);
    return res;
  }

  void PlaceOneType(ShipType type, std::vector<ShipPlacementInfo> & res){
    std::unique_ptr<size_t[]> up_random_location(RandomUnit::NewRandomSequenceArray(kDim * kDim));
    std::unique_ptr<Direction[]> up_random_direction(RandomUnit::NewRandomDirectionArray(kDim * kDim));
    size_t p_current = 0;
    while(trial_board_.CanPlaceMore(type)){
      size_t location = up_random_location.get()[p_current];
      Direction direction = up_random_direction.get()[p_current];

      bool success = trial_board_.PlaceAShip(type, location, direction);
      if(success){
        res.emplace_back(ShipPlacementInfo(type, location, direction));
      }
      p_current++;
    }
  }
};

#endif //BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H
