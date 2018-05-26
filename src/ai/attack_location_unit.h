//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H
#define BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H

#include <memory>
#include <vector>
#include "core/game/game_common.h"
#include "ai/random_unit.h"
#include "ai/probability_board.h"

enum class StrategyAttack{
  kRandom,
  kDFS,
  kProbabilitySimple,
  kDFSProbability
};

class AttackLocationUnit{
public:
  AttackLocationUnit(ImagineBoard & enemy_board):
    ref_enemy_board_(enemy_board),
    probability_board_(enemy_board){
  }

  void UpdateProbabilityBoard(){

#ifdef AI_DEBUG
    // verify the correctness of partial update algorithm
    probability_board_.UpdateProbabilityByLastAttackLocation();
    ProbabilityBoard new_prob_board(ref_enemy_board_);
    for(size_t i = 0; i < kDim * kDim; ++i){
      Logger(std::to_string(i) + "," + std::to_string(probability_board_.GetProbability(i)) + "->" + std::to_string(new_prob_board.GetProbability(i)));
      assert(probability_board_.GetProbability(i) == new_prob_board.GetProbability(i));
    }
#endif

    // if we use probability strategy for every move,
    // we can UpdateProbabilityByLastAttackLocation
    // or else we need to recalculate probability every time

    //probability_board_.UpdateProbabilityByLastAttackLocation();
    probability_board_.RecalculateProbability();
  }

  std::size_t NextAttackLocation(const StrategyAttack & strategy){
    switch(strategy){
      case StrategyAttack ::kRandom:{
        return NextAttackLocationRandom();
      }
      case StrategyAttack ::kDFS:{
        return NextAttackLocationDFS();
      }
      case StrategyAttack ::kProbabilitySimple:{
        return NextAttackLocationProbabilitySimple();
      }
      case StrategyAttack ::kDFSProbability:{
        return NextAttackLocationDFSAndProbability();
      }
      default:{
        assert(false);
      }
    }
  }

  const ProbabilityBoard& GetRefProbBoard(){
    return probability_board_;
  }
private:
  ImagineBoard & ref_enemy_board_;

  // for DFS strategy
  std::vector<size_t> target_location_stack_;

  // for Probability strategy
  ProbabilityBoard probability_board_;

  // attack strategies
  std::size_t NextAttackLocationRandom(){
    auto locations = ref_enemy_board_.GetUnAttackedLocations();
    size_t rand = RandomUnit::GetRandomSizeT(0, locations.size() - 1);
    return locations[rand];
  }

  // if the last fire success and there is no ship sink
  // put the surrounding four location (if not attacked before) to the target_location_stack_
  // if the last fire fail or there is a ship sink, then do nothing.

  // check target stack before doing random search
  // this is essentially a DFS search.
  std::size_t NextAttackLocationDFS(){
    if(ref_enemy_board_.last_attack_success_ && ref_enemy_board_.last_attack_sink_ship_type_ == ShipType::kNotAShip){
      std::vector<size_t> new_targets = ref_enemy_board_.GetSurroundingFourUnAttacked(ref_enemy_board_.last_attack_location_);
      // append to the end of the stack
      target_location_stack_.insert(target_location_stack_.end(), new_targets.begin(), new_targets.end());
    }

    while(!target_location_stack_.empty()){
      size_t location = target_location_stack_.back();
      target_location_stack_.pop_back();
      if(!ref_enemy_board_.LocationAttacked(location)) return location;
    }

    return NextAttackLocationRandom();
  }

  // probability attack
  // determine which location is the most probable to be occupied
  // we start with simple probability analysis:
  // try to place each of the ship to every location, in every direction
  // one successful placement increment the probability of all spots the ship occupied
  std::size_t NextAttackLocationProbabilitySimple(){
    return probability_board_.GetOneHighestProbabilityLocation();

  }


  // the combination attack of DFS and probability
  std::size_t NextAttackLocationDFSAndProbability(){
    if(ref_enemy_board_.last_attack_success_ && ref_enemy_board_.last_attack_sink_ship_type_ == ShipType::kNotAShip){
      std::vector<size_t> new_targets = ref_enemy_board_.GetSurroundingFourUnAttacked(ref_enemy_board_.last_attack_location_);
      // append to the end of the stack
      target_location_stack_.insert(target_location_stack_.end(), new_targets.begin(), new_targets.end());
    }

    while(!target_location_stack_.empty()){
      size_t location = target_location_stack_.back();
      target_location_stack_.pop_back();
      if(!ref_enemy_board_.LocationAttacked(location)) return location;
    }

    return NextAttackLocationProbabilitySimple();
  }


};

#endif //BATTLESHIP_GAME_ATTACK_LOCATION_UNIT_H
