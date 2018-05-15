// Created by Chuilian Kong May 1, 2018

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>
#include "core/board.h"
#include "utils/utils.h"
#include "client/client_talker.h"
#include "client/client_brain.h"

enum class ClientState{
  kStarted,
  kConnected,
  kReady,
  kFire,
  kWait,
  kEndGame
};

class client{
public:
  client(const std::string & server_ip, std::size_t port, GameId game_id):
    cli_talker_(server_ip, port, game_id),
    cli_brain_(my_board_),
    state_(ClientState::kStarted){
  }

  void run(){
    while(true){
      switch (state_) {
        case ClientState::kStarted:{
          // conncet to the game room
          ConnectToGame();
          ChangeStateTo(ClientState::kConnected);
          break;
        }
        case ClientState::kConnected:{
          // place ships
          PlaceShips();
          ChangeStateTo(ClientState::kReady);
          break;
        }
        case ClientState::kReady:{
          // send ready message and wait game start signal from server
          bool first_fire = WaitGameStartSignal();
          if(first_fire){
            ChangeStateTo(ClientState::kFire);
          }else{
            ChangeStateTo(ClientState::kWait);
          }
          break;
        }
        case ClientState::kFire:{
          bool win = MakeOneMove();
          if(win) {
            Logger("client wins the game.");
            ChangeStateTo(ClientState::kEndGame);
          }
          ChangeStateTo(ClientState::kWait);
          break;
        }
        case ClientState::kWait:{
          bool lose = WaitForEnemyAndReplyWithResult();
          if(lose){
            Logger("client loses the game.");
            ChangeStateTo(ClientState::kEndGame);
          }
          ChangeStateTo(ClientState::kFire);
          break;
        }
        case ClientState::kEndGame:{
          Logger("game over.");
          CleanUp();
          return;
        }
        default:{
          assert(false);
        }
      } // end of switch statement
    } // end of while loop
  }
private:
  Board my_board_;
  ClientTalker cli_talker_;
  ClientBrain cli_brain_;

  // game state
  ClientState state_;

  void ChangeStateTo(ClientState new_state){
    state_ = new_state;
  }

  void ConnectToGame(){
    cli_talker_.JoinGame();
  }

  void PlaceShips(){
    std::vector<ShipPlacementInfo> plan = cli_brain_.GenerateShipPlacingPlan(StrategyPlaceShip::kFixed);
    for(auto placement : plan){
      if(cli_talker_.PlaceAShip(placement.type, placement.head_location, placement.direction)){
        my_board_.PlaceAShip(placement.type, placement.head_location, placement.direction);
      }else{
        Logger("place ship fail");
        assert(false);
      }
    }
  }

  // return first fire or not
  bool WaitGameStartSignal(){
    return cli_talker_.SendReadyAndWaitStart();
  }

  // return true if win
  bool MakeOneMove(){
    AttackResult res = cli_talker_.Attack(cli_brain_.GenerateNextAttackLocation(StrategyAttack::kRandom));
    if(res.attacker_win) return true;
    cli_brain_.DigestAttackResult(res);
    return false;
  }

  // return true if lose
  bool WaitForEnemyAndReplyWithResult(){
    AttackResult res = my_board_.Attack(cli_talker_.GetEnemyMove());
    if(res.attacker_win) return true;
    cli_talker_.SendAttackResult(res.success, res.sink_ship_type, res.attacker_win);
    return false;
  }

  // TODO
  void CleanUp(){

  }

};


#endif  // CLIENT_CLIENT_H_
