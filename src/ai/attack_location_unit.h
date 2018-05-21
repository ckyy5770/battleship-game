//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H
#define BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H

#include <memory>
#include "core/game/game_common.h"
#include "ai/random_unit.h"

enum class StrategyAttack{
  kRandom
};

class AttackLocationUnit{
public:
  AttackLocationUnit():
    random_sequence_(RandomUnit::NewRandomSequence(kDim * kDim)){
  }

  std::size_t NextAttackLocation(const StrategyAttack & strategy){
    switch(strategy){
      case StrategyAttack ::kRandom:{
        return NextAttackLocationRandom();
      }
      default:{
        assert(false);
      }
    }
  }
private:
  // for GenerateNextAttackLocationRandom
  std::unique_ptr<size_t[]> random_sequence_;
  std::size_t sequence_ptr_ = 0;


  // attack strategies
  std::size_t NextAttackLocationRandom(){
    return random_sequence_[sequence_ptr_++];
  }
};

#endif //BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H
