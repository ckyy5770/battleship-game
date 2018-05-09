// Created by Chuilian Kong April 30, 2018

#ifndef CORE_NETWORKING_H_
#define CORE_NETWORKING_H_

// asio stand alone mode
#define ASIO_STANDALONE

#include <cstring>
#include "asio.hpp"
#include "utils/utils.h"

static const std::size_t kMaxBufferLength = 256;

enum class MessageType : unsigned char {
  kRequestJoinGame,
  kRequestPlaceAShip,
  kRequestReady,
  kReplyJoinGame,
  kReplyPlaceAShip,
  kReplyStartGame
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
  // an implementation of converting typed data to byte array -- using memcpy
  std::memcpy(arr + offset, &data, sizeof(T));
}

template<typename T>
static void WriteToByteArray(unsigned char* arr, std::size_t offset, T data){
  //WriteToByteArrayImplBitOpts<T>(arr, offset, data);
  WriteToByteArrayImplMemcpy<T>(arr, offset, data);
}

// read type from the byte array
template<typename T>
static void ReadFromByteArrayImplBitOpts(unsigned char* arr, std::size_t offset, T* data){
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
static void ReadFromByteArrayImplMemcpy(unsigned char* arr, std::size_t offset, T* data){
  // an implementation of converting byte array to typed data -- using memcpy
  std::memcpy(data, arr + offset, sizeof(T));
}

template<typename T>
static void ReadFromByteArray(unsigned char* arr, std::size_t offset, T* data){
  //ReadFromByteArrayImplBitOpts<T>(arr, offset, data);
  ReadFromByteArrayImplMemcpy<T>(arr, offset, data);
}

// functions for serializing and deserializing request/reply messages
static void MakeRequestJoinGame(unsigned char* request, std::size_t* length, ClientId cli_id, GameId game_id){
  // REQUEST_JOIN_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t offset = 0;
  request[offset] = static_cast<unsigned char>(MessageType::kRequestJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(request, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(request, offset, game_id);
  offset += sizeof(game_id);
  *length = offset;

  // REMAINING_BYTES
  request[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestJoinGame(unsigned char* request, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(request, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(request, offset, game_id);
}

static void ResolveReplyJoinGame(unsigned char* buffer, std::size_t length, bool* success){
  // SUCCEED_OR_NOT (1 Byte)
  assert(length <= kMaxBufferLength);
  ReadFromByteArray<bool>(buffer, 0, success);
}

static void MakeRequestPlaceAShip(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id, ShipType type, std::size_t head_location, Direction direction){
  // REQUEST_PLACE_A_SHIP (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | SHIP_TYPE (4 Byte) | LOCATION (4 Byte) | DIRECTION (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kRequestPlaceAShip);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(game_id);
  WriteToByteArray<ShipType>(buffer, offset, type);
  offset += sizeof(ShipType);
  WriteToByteArray<std::size_t>(buffer, offset, head_location);
  offset += sizeof(std::size_t);
  WriteToByteArray<Direction>(buffer, offset, direction);
  offset += sizeof(Direction);

  *length = offset;

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestPlaceAShip(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id, ShipType* type, std::size_t* head_location, Direction* direction){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | LOCATION (4 Byte) | DIRECTION (1 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(GameId);
  ReadFromByteArray<ShipType>(buffer, offset, type);
  offset += sizeof(ShipType);
  ReadFromByteArray<std::size_t>(buffer, offset, head_location);
  offset += sizeof(std::size_t);
  ReadFromByteArray<Direction>(buffer, offset, direction);
}

static void ResolveReplyPlaceAShip(unsigned char* buffer, std::size_t length, bool* success){
  // SUCCEED_OR_NOT (1 Byte)
  assert(length <= kMaxBufferLength);
  ReadFromByteArray<bool>(buffer, 0, success);
}


static void MakeRequestReady(unsigned char* request, std::size_t* length, ClientId cli_id, GameId game_id){
  // REQUEST_READY (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t offset = 0;
  request[offset] = static_cast<unsigned char>(MessageType::kRequestReady);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(request, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(request, offset, game_id);
  offset += sizeof(game_id);
  *length = offset;

  // REMAINING_BYTES
  request[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestReady(unsigned char* request, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(request, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(request, offset, game_id);
}

static void ResolveReplyStartGame(unsigned char* buffer, std::size_t length, bool* success){
  // SUCCEED_OR_NOT (1 Byte)
  assert(length <= kMaxBufferLength);
  ReadFromByteArray<bool>(buffer, 0, success);
}


#endif  // CORE_NETWORKING_H_
