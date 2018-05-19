// Created by Chuilian Kong May 16, 2018

#ifndef CORE_NETWORKING_MESSAGES_H_
#define CORE_NETWORKING_MESSAGES_H_

#include "utils/utils.h"
#include "core/networking/serialization.h"
#include "client/client_common.h"
#include "core/game/game_common.h"

using namespace serialization;

static const std::size_t kMaxBufferLength = 256;

enum class MessageType : unsigned char {
  kRequestAttack,
  kReplyAttack,
  kInfoGameId,
  kInfoReady,
  kInfoRoll
};

static std::string MessageTypeToString(const MessageType type){
  switch(type){
    case MessageType::kRequestAttack:{
      return "kRequestAttack";
    }
    case MessageType::kReplyAttack:{
      return "kReplyAttack";
    }
    case MessageType::kInfoGameId:{
      return "kInfoGameId";
    }
    case MessageType::kInfoReady:{
      return "kInfoReady";
    }
    case MessageType::kInfoRoll:{
      return "kInfoRoll";
    }
    default:{
      return "UnknownType";
    }
  }
}

// ******************************************************************
// functions for serializing and deserializing messages
// ******************************************************************

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

  *length = offset;
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
  // REPLY_ATTACK (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | SUCCEED_OR_NOT (1 Byte) | SINK_SHIP_TYPE_DURING_ATTACK (1 Byte) | ATTACKER_WIN_OR_NOT (1 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kReplyAttack);

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

  *length = offset;
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


static void MakeInfoReady(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id){
  // INFO_READY (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kInfoReady);

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

static void ResolveInfoReady(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id){
  // CLIENT_ID (4 Byte) | GAME_ID (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
}


static void MakeInfoRoll(unsigned char* buffer, std::size_t* length, ClientId cli_id, GameId game_id, unsigned long roll_number){
  // INFO_READY (1 Byte) | MESSAGE_REMAINING_BYTES (1 Byte) | CLIENT_ID (4 Byte) | GAME_ID (4 Byte) | ROLL_NUMBER (4 Byte)
  std::size_t offset = 0;
  buffer[offset] = static_cast<unsigned char>(MessageType::kInfoRoll);

  // request[1] is reserved for REMAINING_BYTES
  offset += 2;

  WriteToByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  WriteToByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(GameId);
  WriteToByteArray<unsigned long>(buffer, offset, roll_number);
  offset += sizeof(unsigned long);
  // REMAINING_BYTES
  buffer[1] = static_cast<unsigned char>(offset - 2);

  *length = offset;
  assert(*length <= kMaxBufferLength);
}

static void ResolveInfoRoll(unsigned char* buffer, std::size_t length, ClientId* cli_id, GameId* game_id, unsigned long* roll_number){
  // CLIENT_ID (4 Byte) | GAME_ID (4 Byte) | ROLL_NUMBER (4 Byte)
  assert(length <= kMaxBufferLength);
  std::size_t offset = 0;
  ReadFromByteArray<ClientId>(buffer, offset, cli_id);
  offset += sizeof(ClientId);
  ReadFromByteArray<GameId>(buffer, offset, game_id);
  offset += sizeof(GameId);
  ReadFromByteArray<unsigned long>(buffer, offset, roll_number);
}
#endif  // CORE_NETWORKING_MESSAGES_H_
