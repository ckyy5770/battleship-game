// Created by Chuilian Kong April 30, 2018

#ifndef CORE_NETWORKING_H_
#define CORE_NETWORKING_H_

// asio stand alone mode
#define ASIO_STANDALONE

#include "asio.hpp"
#include "utils/utils.h"

enum class MessageType : unsigned char {
  kRequestJoinGame
};


// functions for serializing and deserializing request/reply messages
static void MakeRequestJoinGame(char* request, ClientId cli_id, GameId game_id){
  // REQUEST_JOIN_GAME (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t p = 0;
  request[p++] = static_cast<unsigned char>(MessageType::kRequestJoinGame);
  std::size_t cli_id_size = sizeof(ClientId);
  ClientId mask =




}

#endif  // CORE_NETWORKING_H_
