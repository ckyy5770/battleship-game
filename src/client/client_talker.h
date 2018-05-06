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
  ClientTalker(std::string server_ip, std::size_t server_port):
    tcp_sock(io_service){
      tcp::endpoint game_server(asio::ip::address::from_string(server_ip), server_port);
      try{
        tcp_sock.connect(game_server);
      }catch(std::exception& e){
        std::cerr << "Can't connect to the server: " << e.what() << "\n";
        assert(false);
      }
  }

  void Start(){

  }

  void JoinGame(){

  }
private:
  asio::io_service io_service;
  tcp::socket tcp_sock;

};

#endif  // CLIENT_CLIENT_TALKER_H_
