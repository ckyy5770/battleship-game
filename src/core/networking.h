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
  kRequestAttack,
  kReplyJoinGame,
  kReplyPlaceAShip,
  kReplyStartGame,
  kReplyAttack,

  kInfoGameId,
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

// ******************************************************************
// functions for serializing and deserializing request/reply messages
// ******************************************************************
static void MakeRequestJoinGame(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id){
  // REQUEST_JOIN_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kRequestJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(game_id);
  *length = offset;

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestJoinGame(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
}

static void MakeReplyJoinGame(unsigned char* buffer, std::size_t* length, bool success){
  // REPLY_JOIN_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kReplyJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<bool>(buffer, offset, success);
  offset += sizeof(bool);

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
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

static void MakeReplyPlaceAShip(unsigned char* buffer, std::size_t* length, bool success){
  // REPLY_PLACE_A_SHIP (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kReplyJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<bool>(buffer, offset, success);
  offset += sizeof(bool);

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveReplyPlaceAShip(unsigned char* buffer, std::size_t length, bool* success){
  // SUCCEED_OR_NOT (1 Byte)
  assert(length <= kMaxBufferLength);
  ReadFromByteArray<bool>(buffer, 0, success);
}


static void MakeRequestReady(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id){
  // REQUEST_READY (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kRequestReady);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(game_id);
  *length = offset;

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestReady(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
}

static void MakeReplyStartGame(unsigned char* buffer, std::size_t* length, bool first_fire){
  // REPLY_START_GAME (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kReplyJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<bool>(buffer, offset, first_fire);
  offset += sizeof(bool);

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveReplyStartGame(unsigned char* buffer, std::size_t length, bool* first_fire){
  // SUCCEED_OR_NOT (1 Byte)
  assert(length <= kMaxBufferLength);
  ReadFromByteArray<bool>(buffer, 0, first_fire);
}

static void MakeRequestAttack(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id, std::size_t location){
  // `REQUEST_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | LOCATION (4 Byte)`
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kRequestAttack);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(game_id);
  WriteToByteArray<std::size_t>(buffer, offset, location);
  offset += sizeof(std::size_t);
  *length = offset;

  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}

static void ResolveRequestAttack(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id, std::size_t* location){
  // CLIENT_ID (4 Byte) | SECRET_KEY (4 Byte) | LOCATION (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(GameId);
  ReadFromByteArray<std::size_t>(buffer, offset, location);
}

static void MakeReplyAttack(unsigned char* buffer, std::size_t* length, bool success, ShipType type, bool attacker_win){
  // REPLY_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte) | SINK_SHIP_TYPE_DURING_ATTACK (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kReplyJoinGame);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<bool>(buffer, offset, success);
  offset += sizeof(bool);
  WriteToByteArray<ShipType>(buffer, offset, type);
  offset += sizeof(ShipType);
  WriteToByteArray<bool>(buffer, offset, attacker_win);
  offset += sizeof(bool);
  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  assert(*length <= kMaxBufferLength);
}


static void ResolveReplyAttack(unsigned char* buffer, std::size_t length, bool* success, ShipType* type, bool* attacker_win){
  // SUCCEED_OR_NOT (1 Byte) | SINK_SHIP_TYPE_DURING_ATTACK (1 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<bool>(buffer, offset, success);
  offset += sizeof(bool);
  ReadFromByteArray<ShipType>(buffer, offset, type);
  offset += sizeof(ShipType);
  ReadFromByteArray<bool>(buffer, offset, attacker_win);
}


static void MakeInfoGameId(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id){
  // INFO_GAME_ID (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kInfoGameId);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(GameId);
  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  *length = offset;
  assert(*length <= kMaxBufferLength);
}

static void ResolveInfoGameId(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | GAME_ID (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
}



#endif  // CORE_NETWORKING_H_
