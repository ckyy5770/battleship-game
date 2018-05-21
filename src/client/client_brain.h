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
#include "ai/ship_placement_unit.h"
#include "ai/attack_location_unit.h"

class ClientBrain{
public:
  ClientBrain(Board& client_board):
    my_board_(client_board){
  }

  std::vector<ShipPlacementInfo> GenerateShipPlacingPlan(const StrategyPlaceShip& strategy){
    switch (strategy) {
      case StrategyPlaceShip::kFixed:{
        return ship_placement_unit_.ShipPlacingPlan(strategy);
      }
      default:{
        assert(false);
      }
    }
  }

  std::size_t GenerateNextAttackLocation(const StrategyAttack& strategy){
    switch (strategy) {
      case StrategyAttack::kRandom:{
        return attack_location_unit_.NextAttackLocation(strategy);
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

  ShipPlacementUnit ship_placement_unit_;
  AttackLocationUnit attack_location_unit_;

};

#endif  // CLIENT_CLIENT_BRAIN_H_
