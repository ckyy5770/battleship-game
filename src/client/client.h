// Created by Chuilian Kong May 1, 2018

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "core/board.h"
#include "utils/utils.h"

enum class ClientState{
  kStarted,
  kConnected,
  kReady,
  kInGame
};

class client{
public:
  client(const std::string & server_ip, std::size_t port, int game_id)
    : server_ip_(server_ip),
      port_(port),
      game_id_(game_id),
      state_(ClientState::kStarted){

      ConfigMyID();
  }

  void run(){
    switch (state_) {
      case ClientState::kStarted:{
        // TODO: conncet to the game room
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
  std::string server_ip_;
  std::size_t port_;
  int my_id_;
  Board my_board_;

  // the unique game id, this should be given in cmd
  int game_id_;

  // game state
  ClientState state_;

  // assign this client a unique 4 Byte id
  // typically this should be done by applying one from server
  // but for simplisity, we just give it a random number
  void ConfigMyID(){
    std::srand(std::time(nullptr));
    my_id_ = std::rand();
  }

  void ChangeStateTo(ClientState new_state){
    state_ = new_state;
  }
};


#endif  // CLIENT_CLIENT_H_
