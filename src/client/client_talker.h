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
    tcp_sock_(io_service_){
      tcp::endpoint game_server(asio::ip::address::from_string(server_ip), server_port);
      try{
        tcp_sock_.connect(game_server);
      }catch(std::exception& e){
        std::cerr << "Can't connect to the server: " << e.what() << "\n";
        assert(false);
      }
  }


  void Start(){

  }



  void JoinGame(){
    //asio::write(tcp_sock_, asio::buffer(request, request_length));

  }
private:
  static const std::size_t kMaxLength = 1024;
  asio::io_service io_service_;
  tcp::socket tcp_sock_;

};

#endif  // CLIENT_CLIENT_TALKER_H_
