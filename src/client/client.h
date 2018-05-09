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
  kInGame
};

class client{
public:
  client(const std::string & server_ip, std::size_t port, GameId game_id):
    cli_talker_(server_ip, port, game_id),
    cli_brain_(my_board_),
    state_(ClientState::kStarted){
  }

  void run(){
    switch (state_) {
      case ClientState::kStarted:{
        // conncet to the game room
        cli_talker_.JoinGame();
        ChangeStateTo(ClientState::kConnected);
        break;
      }
      case ClientState::kConnected:{
        // TODO: place ships
        break;
      }
      case ClientState::kReady:{
        // TODO: wait game start signal from server
        break;
      }
      case ClientState::kInGame:{
        // TODO: make one move, and wait for result,
        //       wait for enemy's move, update the state
        //       make another move.
        break;
      }
      default:{
        assert(false);
      }
    }

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


};


#endif  // CLIENT_CLIENT_H_
