// Created by Chuilian Kong May 14, 2018

#ifndef SERVER_SERVER_TALKER_H_
#define SERVER_SERVER_TALKER_H_

#include <chrono>
#include <thread>
#include "core/networking.h"
#include "server/game_state_controller.h"
#include "server/server_talker_controller.h"

using asio::ip::tcp;

class ServerTalkerController;

enum class ServerTalkerState{
  kStarted,
  kConnected,
  kFire,
  kWait,
  kEndGame
};

class ServerTalker{
public:
  // you need to pass std::move(socket) to this ctor
  ServerTalker(tcp::socket socket, GameStateController& state_controller, ServerTalkerController& talker_controller):
    tcp_sock_(std::move(socket)),
    game_state_controller_(state_controller),
    server_talker_controller_(talker_controller){
  }

  void Run(){
    while(true){
      switch (server_talker_state_) {
        case ServerTalkerState::kStarted:{
          // init client id and game id from the first message
          EstablishConnection();
          RegisterThisTalker();
          ChangeStateTo(ServerTalkerState::kConnected);
          break;
        }
        case ServerTalkerState::kConnected:{
          // wait for client make place ship requests,
          // and also wait for the ready request
          ExpectPlaceAShipAndReady();
          bool first_fire = CheckWhoFireFirst();
          if(first_fire){
            ChangeStateTo(ServerTalkerState::kFire);
          }else{
            ChangeStateTo(ServerTalkerState::kWait);
          }
          break;
        }
        case ServerTalkerState::kFire:{
          // wait for fire request from the client
          // re-send the request to the opponent
          ExpectAttack();
          // we've changed my state in ExpectAttack()
          break;
        }
        case ServerTalkerState::kWait:{
          // wait for signal of sending a attack message
          WaitForAttack();
          // we don't change my state, my opponent will change for me,
          // when they finish a fire
          break;
        }
        case ServerTalkerState::kEndGame:{
          // TODO: there is no way to get to this state for now.
          CleanUp();
          break;
        }
        default:{
          assert(false);
        }
      }

    }
  }

private:
  tcp::socket tcp_sock_;
  GameStateController& game_state_controller_;
  ServerTalkerController& server_talker_controller_;

  // corresponding client id of this server talker
  ClientId cli_id_;

  // corresponding game id of this server talker
  GameId game_id_;

  // state
  ServerTalkerState server_talker_state_;

  void SendMessage(unsigned char* message, std::size_t length){
    asio::write(tcp_sock_, asio::buffer(message, length));
  }

  void ChangeStateTo(ServerTalkerState new_state){
    server_talker_state_ = new_state;
  }

  void EstablishConnection(){
    // kRequestJoinGame should be the very first message the client send to server
    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kRequestJoinGame);
    unsigned char message_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(message_body, length));
    ResolveRequestJoinGame(message_body, length, &cli_id_, &game_id_);
    // servser state change
    ThisClientJoinGame();
  }

  void ExpectPlaceAShipAndReady(){
    MessageType type = GetMessageType();
    std::size_t length = GetBodyLength();
    switch (type) {
      case MessageType::kRequestPlaceAShip:{
        unsigned char request_body[kMaxBufferLength];
        asio::read(tcp_sock_, asio::buffer(request_body, length));
        ClientId cli_id;
        GameId game_id;
        ShipType type;
        std::size_t head_location;
        Direction direction;
        ResolveRequestPlaceAShip(request_body, length, &cli_id, &game_id, &type, &head_location, &direction);
        EnsureClientIntegrity(cli_id, game_id);
        // handle place ship request
        // TODO: for now, the server doesn't keep a copy of player's board. It simply gives a positive reply.
        unsigned char reply[kMaxBufferLength];
        std::size_t reply_length = 0;
        MakeReplyPlaceAShip(reply, &reply_length, true);
        asio::write(tcp_sock_, asio::buffer(reply, reply_length));
        break;
      }
      case MessageType::kRequestReady:{
        unsigned char request_body[kMaxBufferLength];
        asio::read(tcp_sock_, asio::buffer(request_body, length));
        ClientId cli_id;
        GameId game_id;
        ResolveRequestReady(request_body, length, &cli_id, &game_id);
        EnsureClientIntegrity(cli_id, game_id);

        MakeThisReadyAndWaitForTheOther();

        unsigned char reply[kMaxBufferLength];
        std::size_t reply_length = 0;
        MakeReplyStartGame(reply, &reply_length, true);
        asio::write(tcp_sock_, asio::buffer(reply, reply_length));
        break;
      }
      default:{
        Logger("unexpected client request type");
        assert(false);
      }
    }
  }

  void ExpectAttack(){
    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kRequestAttack);
    unsigned char request_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(request_body, length));
    ClientId cli_id;
    GameId game_id;
    std::size_t location;
    ResolveRequestAttack(request_body, length, &cli_id, &game_id, &location);
    EnsureClientIntegrity(cli_id, game_id);

    // trigger the opponent socket to send a attack message.
    ServerTalker* oppo_talker = server_talker_controller_.GetTalker(game_state_controller_.GetTheOther(cli_id_, game_id_));
    // rewrite the attack request and send it
    unsigned char request[kMaxBufferLength];
    std::size_t request_length = 0;
    MakeRequestAttack(request, &request_length, cli_id, game_id, location);
    oppo_talker -> SendMessage(request, request_length);


    // change my state first!
    ChangeStateTo(ServerTalkerState::kWait);
    // change the opponent talker state to fire!
    oppo_talker -> ChangeStateTo(ServerTalkerState::kFire);
  }

  void WaitForAttack(){
    // Just sleep and DO NOT use your socket.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  // TODO
  void CleanUp(){

  };

  void RegisterThisTalker(){
    server_talker_controller_.Register(cli_id_, this);
  }

  void ThisClientJoinGame(){
    game_state_controller_.JoinGame(cli_id_, game_id_);
  }

  void MakeThisReadyAndWaitForTheOther(){
    game_state_controller_.PlayerReady(cli_id_, game_id_);
    while(true){
      if(game_state_controller_.IsTheOtherReady(cli_id_, game_id_)){
        break;
      }
      // check the ready state of the other player every 0.5 seconds.
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }

  bool CheckWhoFireFirst(){
    return game_state_controller_.CheckWhoFireFirst(game_id_) == cli_id_;
  }


  MessageType GetMessageType(){
    unsigned char reply_type[1];
    asio::read(tcp_sock_, asio::buffer(reply_type, 1));
    return static_cast<MessageType>(reply_type[0]);
  }

  std::size_t GetBodyLength(){
    unsigned char reply_length[1];
    asio::read(tcp_sock_, asio::buffer(reply_length, 1));
    assert(static_cast<std::size_t>(reply_length[0]) < kMaxBufferLength);

    return static_cast<std::size_t>(reply_length[0]);
  }

  std::size_t EnsureMessageTypeAndGetBodyLength(MessageType type){
    unsigned char reply_type[1];
    asio::read(tcp_sock_, asio::buffer(reply_type, 1));
    if(reply_type[0] != static_cast<unsigned char>(type)){
      Logger("unexpected reply type");
      assert(false);
    }

    unsigned char reply_length[1];
    asio::read(tcp_sock_, asio::buffer(reply_length, 1));
    assert(static_cast<std::size_t>(reply_length[0]) < kMaxBufferLength);

    return static_cast<std::size_t>(reply_length[0]);
  }

  void EnsureClientIntegrity(ClientId cli_id, GameId game_id){
    assert(cli_id == cli_id_);
    assert(game_id == game_id_);
  }

};

#endif  // SERVER_SERVER_TALKER_H_
