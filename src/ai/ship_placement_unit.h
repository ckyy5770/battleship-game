//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H
#define BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H

#include <vector>
#include "core/game/board.h"

enum class StrategyPlaceShip{
  kFixed
};

struct ShipPlacementInfo{
  ShipType type;
  std::size_t head_location;
  Direction direction;

  ShipPlacementInfo(){};
  ShipPlacementInfo(ShipType type, std::size_t head_location, Direction direction):
    type(type),
    head_location(head_location),
    direction(direction){};
};


class ShipPlacementUnit{
public:
  ShipPlacementUnit(){}

  std::vector<ShipPlacementInfo> ShipPlacingPlan(const StrategyPlaceShip & strategy){
    switch(strategy){
      case StrategyPlaceShip::kFixed:{
        return ShipPlacingPlanFixed();
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
};

#endif //BATTLESHIP_GAME_SHIP_PLACEMENT_UNIT_H
