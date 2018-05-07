// Created by Chuilian Kong April 30, 2018

#ifndef CORE_NETWORKING_H_
#define CORE_NETWORKING_H_

// asio stand alone mode
#define ASIO_STANDALONE

#include <cstring>
#include "asio.hpp"
#include "utils/utils.h"

enum class MessageType : unsigned char {
  kRequestJoinGame
};

// write type to the byte array
template<typename T>
static void WriteToByteArrayImplBitOpts(unsigned char* arr, std::size_t offset, T data){
  // an implementation of converting typed data to byte array -- using bit operations
  std::size_t type_size = sizeof(T);
  T mask = 0xFF; // 0000 0000 0000 0000 0000 0000 1111 1111 for 4-byte int
  std::size_t mask_offset = 0;
  for(std::size_t i = 0; i < type_size; i++){
    arr[offset + type_size - 1 - i] = static_cast<unsigned char>((data & (mask << mask_offset)) >> mask_offset);
    mask_offset += 8;
  }
}

template<typename T>
static void WriteToByteArrayImplMemcpy(unsigned char* arr, std::size_t offset, T data){
  // TODO
  // an implementation of converting typed data to byte array -- using memcpy
}

template<typename T>
static void WriteToByteArray(unsigned char* arr, std::size_t offset, T data){
  WriteToByteArrayImplBitOpts(arr, offset, data);
  // WriteToByteArrayImplMemcpy(arr, offset, data);
}

// read type from the byte array
template<typename T>
static void ReadToByteArrayImplBitOpts(unsigned char* arr, std::size_t offset, T* data){
  // an implementation of converting byte array to typed data -- using bit operations
  std::size_t type_size = sizeof(T);
  std::memset(data, 0, type_size);
  std::size_t mask_offset = 0;
  for(std::size_t i = 0; i < type_size; i++){
    *data |= (static_cast<T>(arr[offset + type_size - 1 - i]) << mask_offset);
    mask_offset += 8;
  }
}

template<typename T>
static void ReadToByteArrayImplMemcpy(unsigned char* arr, std::size_t offset, T* data){
  // TODO
  // an implementation of converting byte array to typed data -- using memcpy
}

template<typename T>
static void ReadFromByteArray(unsigned char* arr, std::size_t offset, T* data){
  ReadToByteArrayImplBitOpts(arr, offset, data);
  //ReadToByteArrayImplMemcpy(arr, offset, data);
}

// functions for serializing and deserializing request/reply messages
static void MakeRequestJoinGame(unsigned char* request, ClientId cli_id, GameId game_id){
  // REQUEST_JOIN_GAME (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t offset = 0;
  request[offset++] = static_cast<unsigned char>(MessageType::kRequestJoinGame);
  WriteToByteArray<ClientId>(request, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(request, offset, cli_id);
}

#endif  // CORE_NETWORKING_H_
