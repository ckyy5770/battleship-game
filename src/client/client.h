// Created by Chuilian Kong May 1, 2018

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <string>
#include "core/board.h"

class client{
public:
  client(const std::string & server_ip, std::size_t port)
    : server_ip_(server_ip),
      port_(port){

  }
private:
  std::string server_ip_;
  std::size_t port_;
};


#endif  // CLIENT_CLIENT_H_
