// Created by Chuilian Kong May 8, 2018

#ifndef CLIENT_CLIENT_BRAIN_H_
#define CLIENT_CLIENT_BRAIN_H_

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "core/game/game_common.h"
#include "core/game/board.h"
#include "core/game/imagine_board.h"
#include "client/client_talker.h"

enum class StrategyPlaceShip{
  kFixed
};

enum class StrategyAttack{
  kRandom
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
      for(std::size_t i = 0; i < kDim * kDim; i++){
        random_array_[i] = i;
      }
      std::shuffle(random_array_, random_array_ + kDim * kDim, std::default_random_engine(time(nullptr)));
  }

  std::vector<ShipPlacementInfo> GenerateShipPlacingPlan(const StrategyPlaceShip& strategy){
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

  std::size_t GenerateNextAttackLocation(const StrategyAttack& strategy){
    switch (strategy) {
      case StrategyAttack::kRandom:{
        return GenerateNextAttackLocationRandom();
        break;
      }
      default:{
        assert(false);
      }
    }
  }

  void DigestAttackResult(const AttackResult& res){
    opponent_board_.MarkAttack(res.location);

    if(res.success){
      opponent_board_.MarkOccupied(res.location);
      opponent_board_.DestroyOneOnBoard(res.sink_ship_type);
    }

  }

  ImagineBoard & GetRefEnemyBoard(){
    return opponent_board_;
  }


private:
  Board& my_board_;
  ImagineBoard opponent_board_;
  // for GenerateNextAttackLocationRandom
  std::size_t random_array_[kDim * kDim];
  std::size_t array_ptr_ = 0;

  // ship placement strategies
  std::vector<ShipPlacementInfo> GenerateShipPlacingPlanFixed(){
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

  // attack strategies
  std::size_t GenerateNextAttackLocationRandom(){
    return random_array_[array_ptr_++];
  }



};

#endif  // CLIENT_CLIENT_BRAIN_H_
