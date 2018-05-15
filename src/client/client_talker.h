// Created by Chuilian Kong May 5, 2018

#ifndef CLIENT_CLIENT_TALKER_H_
#define CLIENT_CLIENT_TALKER_H_

#include <iostream>
#include <string>
#include "core/networking.h"
#include "utils/utils.h"
using asio::ip::tcp;

// This object will handle all network requests & response to & from the server
class ClientTalker{
public:
  ClientTalker(std::string server_ip, std::size_t server_port, GameId game_id):
    tcp_sock_(io_service_),
    cli_id_(0),
    game_id_(game_id){
      tcp::endpoint game_server(asio::ip::address::from_string(server_ip), server_port);
      try{
        tcp_sock_.connect(game_server);
      }catch(std::exception& e){
        std::cerr << "Can't connect to the server: " << e.what() << "\n";
        assert(false);
      }

      ConfigClientId();
  }

  void JoinGame(){
    unsigned char request[kMaxBufferLength];
    std::size_t request_length = 0;
    MakeRequestJoinGame(request, &request_length, cli_id_, game_id_);
    asio::write(tcp_sock_, asio::buffer(request, request_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kReplyJoinGame);
    unsigned char reply_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(reply_body, length));
    bool success = false;
    ResolveReplyJoinGame(reply_body, length, &success);

    if(!success){
      Logger("join game fail");
      assert(false);
    }
  }

  bool PlaceAShip(ShipType type, std::size_t head_location, Direction direction){
    // send place ship request to server, and get reply
    unsigned char request[kMaxBufferLength];
    std::size_t request_length = 0;
    MakeRequestPlaceAShip(request, &request_length, cli_id_, game_id_, type, head_location, direction);
    asio::write(tcp_sock_, asio::buffer(request, request_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kReplyPlaceAShip);
    unsigned char reply_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(reply_body, length));
    bool success = false;
    ResolveReplyPlaceAShip(reply_body, length, &success);

    return success;
  }

  // return first fire or not
  bool SendReadyAndWaitStart(){
    // notify server this client is ready.
    // wait for game start signal
    unsigned char request[kMaxBufferLength];
    std::size_t request_length = 0;
    MakeRequestReady(request, &request_length, cli_id_, game_id_);
    asio::write(tcp_sock_, asio::buffer(request, request_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kReplyStartGame);
    unsigned char reply_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(reply_body, length));
    bool first_fire = false;
    ResolveReplyStartGame(reply_body, length, &first_fire);

    return first_fire;

  }

  AttackResult Attack(size_t location){
    // send attack request to server, and get reply
    unsigned char request[kMaxBufferLength];
    std::size_t request_length = 0;
    MakeRequestAttack(request, &request_length, cli_id_, game_id_, location);
    asio::write(tcp_sock_, asio::buffer(request, request_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kReplyAttack);
    unsigned char reply_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(reply_body, length));
    bool success = false;
    ShipType sink_ship_type = kNotAShip;
    bool attacker_win = false;
    ResolveReplyAttack(reply_body, length, &success, &sink_ship_type, &attacker_win);

    return AttackResult(location, success, sink_ship_type, attacker_win);
  }

  size_t GetEnemyMove(){
    // get one enemy move
    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kRequestAttack);
    unsigned char request_body[kMaxBufferLength];
    asio::read(tcp_sock_, asio::buffer(request_body, length));
    ClientId client_id = 0;
    GameId game_id = 0;
    size_t location = 0;
    ResolveRequestAttack(request_body, length, &client_id, &game_id, &location);
    return location;
  }

  void SendAttackResult(bool success, ShipType type, bool attacker_win){
    // send attack reply to server
    unsigned char reply[kMaxBufferLength];
    std::size_t reply_length = 0;
    MakeReplyAttack(reply, &reply_length, success, type, attacker_win);
    asio::write(tcp_sock_, asio::buffer(reply, reply_length));
  }

private:
  asio::io_service io_service_;
  tcp::socket tcp_sock_;
  // client id will be retrieved from server after ctor
  ClientId cli_id_;

  // the unique game id, this should be given in cmd
  GameId game_id_;

  // assign this client a unique 4 Byte id
  // typically this should be done by applying one from server
  // but for simplisity, we just give it a random number
  void ConfigClientId(){
    std::srand(std::time(nullptr));
    cli_id_ = std::rand();
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

};

#endif  // CLIENT_CLIENT_TALKER_H_
