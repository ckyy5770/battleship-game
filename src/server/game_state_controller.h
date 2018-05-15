// Created by Chuilian Kong May 14, 2018

#ifndef SERVER_GAME_STATE_CONTROLLER_H_
#define SERVER_GAME_STATE_CONTROLLER_H_

#include <unordered_map>
#include "utils/utils.h"

class GameStateController{
public:
  GameStateController(){
  }

  void JoinGame(const ClientId & cli_id, const GameId & game_id){
    if(!GameExist(game_id)){
      CreateNewGame(game_id);
    }
    GameState & game = game_state_map_.at(game_id);
    const std::size_t player_num = game.player_num;
    game.player_ids[player_num] = cli_id;
  }

  const ClientId & GetTheOther(const ClientId & this_cli, const GameId & game_id) const{
    const GameState & game = game_state_map_.at(game_id);
    assert(GameExist(game_id));
    assert(game.player_num == 2);
    return this_cli == game.player_ids[0] ? game.player_ids[1] : game.player_ids[0];
  }

  void PlayerReady(const ClientId & cli_id, const GameId & game_id){
    GameState & game = game_state_map_.at(game_id);
    std::size_t player_index = GetClientIndex(cli_id, game_id);
    game.ready[player_index] = true;
  }

  bool IsTheOtherReady(const ClientId & cli_id, const GameId & game_id) const{
    const GameState & game = game_state_map_.at(game_id);
    std::size_t player_index = GetTheOtherIndex(cli_id, game_id);
    return game.ready[player_index];
  }

  const ClientId CheckWhoFireFirst(const GameId & game_id) const{
    // TODO for now, player[0] always fire first
    const GameState & game = game_state_map_.at(game_id);
    return game.player_ids[game.first_fire];
  }
private:
  struct GameState{
    GameId game_id;
    ClientId player_ids[2];
    bool ready[2];
    std::size_t player_num;
    std::size_t first_fire;

    explicit GameState(GameId game_id):
      game_id(game_id),
      player_num(0),
      first_fire(0){
        std::memset(player_ids, 0, sizeof(ClientId) * 2);
        std::memset(ready, 0, sizeof(bool) * 2);
      };
  };

  std::unordered_map<GameId, GameState> game_state_map_;


  void CreateNewGame(const GameId & id){
    if(GameExist(id)){
      Logger("the game is already existed.");
      return;
    }
    game_state_map_.insert(std::make_pair(id, GameState(id)));
  }

  bool GameExist(GameId id) const{
    return game_state_map_.find(id) != game_state_map_.end();
  }

  const size_t GetClientIndex (const ClientId & cli_id, const GameId & game_id) const{
    const GameState & game = game_state_map_.at(game_id);
    return cli_id == game.player_ids[0] ? 0 : 1;
  }

  const size_t GetTheOtherIndex (const ClientId & cli_id, const GameId & game_id) const{
    const GameState & game = game_state_map_.at(game_id);
    return cli_id == game.player_ids[0] ? 1 : 0;
  }

};
#endif  //SERVER_GAME_STATE_CONTROLLER_H_
