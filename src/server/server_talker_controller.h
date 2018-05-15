// Created by Chuilian Kong May 14, 2018

#ifndef SERVER_CLIENT_SOCKET_CONTROLLER_H_
#define SERVER_CLIENT_SOCKET_CONTROLLER_H_

#include <unordered_map>
#include "core/networking.h"
#include "server/server_talker.h"

using asio::ip::tcp;

class ServerTalker;

class ServerTalkerController{
public:
  ServerTalkerController(){}
  void Register(ClientId cli_id, ServerTalker* talker){
    if(ClientExist(cli_id)) assert(false);
    talker_map_.insert(std::make_pair(cli_id, talker));
  }

  ServerTalker* GetTalker(ClientId cli_id){
    if(!ClientExist(cli_id)) assert(false);
    return talker_map_.at(cli_id);
  }
private:
  std::unordered_map<ClientId, ServerTalker*> talker_map_;

  bool ClientExist(ClientId id){
    return talker_map_.find(id) != talker_map_.end();
  }
};

#endif  // SERVER_CLIENT_SOCKET_CONTROLLER_H_
