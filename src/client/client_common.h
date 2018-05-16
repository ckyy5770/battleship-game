// Created by Chuilian Kong May 1, 2018

#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

typedef unsigned int ClientId;
typedef unsigned int GameId;


enum class ClientType{
  kInitiator,
  kListener
};

class ClientTalker;
class ClientBrain;
class GameClient;


#endif  // CLIENT_CLIENT_H_
