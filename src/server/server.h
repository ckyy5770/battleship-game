// Created by Chuilian Kong April 30, 2018

#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include <string>
#include <thread>
#include <iostream>
#include "core/networking.h"

using asio::ip::tcp;

class Server{
public:
  Server(std::size_t port)
    : listen_port_(port){
  }

  // this call will block
  void Start(){
    tcp::acceptor acceptor(io_service_, tcp::endpoint(tcp::v4(), listen_port_));
    while (true)
    {
      tcp::socket socket(io_service_);
      acceptor.accept(socket);
      std::thread(&Server::Session, this, std::move(socket)).detach();
    }
  }

private:
  static const std::size_t kMaxLength = 1024;
  std::size_t listen_port_;
  asio::io_service io_service_;

  void Session(tcp::socket socket)
  {
    try
    {
      while(true)
      {
        char request[kMaxLength];
        char reply[kMaxLength];

        asio::error_code error;
        socket.read_some(asio::buffer(request), error);
        if (error == asio::error::eof)
          break; // Connection closed cleanly by peer.
        else if (error)
          throw asio::system_error(error); // Some other error.

        // now we get the request from client, process and respond to it
        ProcessRequest(request, reply);

        asio::write(socket, asio::buffer(reply, sizeof(reply) / sizeof(char)));
      }
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception in thread: " << e.what() << "\n";
    }
  }

  void ProcessRequest(const char * const request, const char * const reply){

  }

};


#endif  // SERVER_SERVER_H_
