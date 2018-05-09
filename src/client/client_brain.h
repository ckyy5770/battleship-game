// Created by Chuilian Kong May 8, 2018

#ifndef CLIENT_CLIENT_BRAIN_H_
#define CLIENT_CLIENT_BRAIN_H_

#include <vector>
#include "core/board.h"
#include "core/ship.h"

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

class ClientBrain{
public:
  ClientBrain(Board& client_board):
    my_board_(client_board){
  }

  std::vector<ShipPlacementInfo> GenerateShipPlacingPlan(StrategyPlaceShip strategy){
    switch (strategy) {
      case StrategyPlaceShip::kFixed:{
        return GenerateShipPlacingPlanFixed();
        break;
      }
      default:{
        assert(false);
      }
    }

  }
private:
  Board& my_board_;
  Board opponent_board_;

  std::vector<ShipPlacementInfo> GenerateShipPlacingPlanFixed(){
    std::vector<ShipPlacementInfo> ret;
    ret.push_back(ShipPlacementInfo(ShipType::kCarrier, 0, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kBattleShip, 1, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kBattleShip, 2, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kCruiser, 3, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kCruiser, 4, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kCruiser, 5, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kDestroyer, 6, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kDestroyer, 7, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kDestroyer, 8, Direction::kVertical));
    ret.push_back(ShipPlacementInfo(ShipType::kDestroyer, 9, Direction::kVertical));

    return ret;
  }

};

#endif  // CLIENT_CLIENT_BRAIN_H_
