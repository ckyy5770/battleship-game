// Created by Chuilian Kong May 5, 2018

#ifndef CLIENT_CLIENT_TALKER_H_
#define CLIENT_CLIENT_TALKER_H_

#include <iostream>
#include <string>
#include "core/game/game_common.h"
#include "core/networking/networking.h"
#include "client/client_common.h"

using asio::ip::tcp;

// This object will handle all network requests & response to & from the server
class ClientTalker{
public:
  ClientTalker(const ClientType & cli_type, const std::string & peer_ip, const std::size_t & port, const ClientId & cli_id, const GameId & game_id):
    tcp_sock_(io_service_),
    cli_id_(cli_id),
    game_id_(game_id){
      switch (cli_type) {
        case ClientType::kInitiator:{
          ConnectToPeer(peer_ip, port);
          break;
        }
        case ClientType::kListener:{
          ListenForConnection(port);
          break;
        }
        default:{
          assert(false);
        }
      }
  }

  GameId SendMyGameIdAndGetTheOther(){
    unsigned char buffer[kMaxBufferLength];
    std::size_t message_length = 0;
    MakeInfoGameId(buffer, &message_length, cli_id_, game_id_);
    asio::write(tcp_sock_, asio::buffer(buffer, message_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kInfoGameId);
    asio::read(tcp_sock_, asio::buffer(buffer, length));
    ClientId cli_id;
    GameId game_id;
    ResolveInfoGameId(buffer, length, &cli_id, &game_id);
    // TODO: maybe we don't need cli_id
    return game_id;
  }

  void SendMyReadyAndWaitTheOther(){
    unsigned char buffer[kMaxBufferLength];
    std::size_t message_length = 0;
    MakeInfoReady(buffer, &message_length, cli_id_, game_id_);
    asio::write(tcp_sock_, asio::buffer(buffer, message_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kInfoReady);
    asio::read(tcp_sock_, asio::buffer(buffer, length));
    ClientId cli_id;
    GameId game_id;
    ResolveInfoReady(buffer, length, &cli_id, &game_id);
    // TODO: maybe we don't need cli_id and game_id?
    // or maybe we can add a verification here.
  }

  bool DecideWhoFireFirst(){
    // TODO for now we use cli_id to decide which one should fire first.
    unsigned long my_num = cli_id_;

    unsigned char buffer[kMaxBufferLength];
    std::size_t message_length = 0;
    MakeInfoRoll(buffer, &message_length, cli_id_, game_id_, my_num);
    asio::write(tcp_sock_, asio::buffer(buffer, message_length));

    std::size_t length = EnsureMessageTypeAndGetBodyLength(MessageType::kInfoRoll);
    asio::read(tcp_sock_, asio::buffer(buffer, length));
    ClientId cli_id;
    GameId game_id;
    unsigned long oppo_num;
    ResolveInfoRoll(buffer, length, &cli_id, &game_id, &oppo_num);

    // TODO only for now
    assert(my_num != oppo_num);

    return my_num > oppo_num;
  }

  AttackResult Attack(size_t location){
    // send attack request to peer, and get reply
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
    // send attack reply to peer
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

  void ConnectToPeer(const std::string & peer_ip, const std::size_t & port){
    tcp::endpoint peer(asio::ip::address::from_string(peer_ip), port);
    try{
      tcp_sock_.connect(peer);
    }catch(std::exception& e){
      std::cerr << "Can't connect to the server: " << e.what() << "\n";
      assert(false);
    }
  }

  void ListenForConnection(const std::size_t & listen_port){
    // we only expect one connection.
    tcp::acceptor acceptor(io_service_, tcp::endpoint(tcp::v4(), listen_port));
    acceptor.accept(tcp_sock_);
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

    Logger("Message Received. type = " + MessageTypeToString(type));

    return static_cast<std::size_t>(reply_length[0]);
  }

};

#endif  // CLIENT_CLIENT_TALKER_H_
