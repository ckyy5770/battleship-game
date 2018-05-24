//
// Created by Kong, Chuilian on 5/16/18.
//

#ifndef BATTLESHIP_CLIENT_GAME_CLIENT_H
#define BATTLESHIP_CLIENT_GAME_CLIENT_H

#include <iostream>
#include "client/client_common.h"
#include "client/client_talker.h"
#include "client_brain.h"
#include "core/game/board.h"
#include "utils/utils.h"

enum class ClientState {
  kStarted,
  kConnected,
  kReady,
  kFire,
  kWait,
  kEndGame
};

static std::string ClientStateToString(const ClientState state){
  switch(state){
    case ClientState::kStarted:{
      return "kStarted";
    }
    case ClientState::kConnected:{
      return "kConnected";
    }
    case ClientState::kReady:{
      return "kReady";
    }
    case ClientState::kFire:{
      return "kFire";
    }
    case ClientState::kWait:{
      return "kWait";
    }
    case ClientState::kEndGame:{
      return "kEndGame";
    }
    default:{
      return "UnknownState";
    }
  }
}

class GameClient {
public:
  GameClient(const ClientType &type, const std::string &peer_ip, const std::size_t &port, const ClientId &cli_id,
             const GameId &game_id, pthread_t main_thread) :
    cli_type_(type),
    cli_id_(cli_id),
    game_id_(game_id),
    cli_talker_(type, peer_ip, port, cli_id, game_id),
    cli_brain_{my_board_},
    state_(ClientState::kStarted),
    main_thread_(main_thread){

  }

  void run() {
    while (true) {
      switch (state_) {
        case ClientState::kStarted: {
          // verify game id.
          VerifyGameId();
          ChangeStateTo(ClientState::kConnected);
          break;
        }
        case ClientState::kConnected: {
          // place ships
          PlaceShips();
          ChangeStateTo(ClientState::kReady);
          break;
        }
        case ClientState::kReady: {
          // wait for both clients are ready and roll a dice to decide
          // who shoot first
          Ready();
          bool first_fire = DecideWhoFireFirst();

          if (first_fire) {
            ChangeStateTo(ClientState::kFire);
          } else {
            ChangeStateTo(ClientState::kWait);
          }
          break;
        }
        case ClientState::kFire: {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          bool win = MakeOneMove();
          if (win) {
            is_winner_me_ = true;
            Logger("client wins the game.");
            ChangeStateTo(ClientState::kEndGame);
            break;
          }
          ChangeStateTo(ClientState::kWait);
          break;
        }
        case ClientState::kWait: {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          bool lose = WaitForEnemyAndReplyWithResult();
          if (lose) {
            is_winner_me_ = false;
            Logger("client loses the game.");
            ChangeStateTo(ClientState::kEndGame);
            break;
          }
          ChangeStateTo(ClientState::kFire);
          break;
        }
        case ClientState::kEndGame: {
          Logger("game over.");
          SetWinnerLoserOnBoards();
          OutputResultAndExit();
          return;
        }
        default: {
          assert(false);
        }
      } // end of switch statement
    } // end of while loop
  }

  Board& GetRefMyBoard(){
    return my_board_;
  }

  ImagineBoard& GetRefEnemyBoard(){
    return cli_brain_.GetRefEnemyBoard();
  }

private:
  const ClientType &cli_type_;
  const ClientId &cli_id_;
  const GameId &game_id_;

  Board my_board_;
  ClientTalker cli_talker_;
  ClientBrain cli_brain_;

  bool is_winner_me_ = false;

  // game state
  ClientState state_;

  // TODO: we have to use signal to interrupt ui thread (main thread), there maybe better way
  pthread_t main_thread_;


  void ChangeStateTo(ClientState new_state) {
    Logger(ClientStateToString(state_) + " change to " + ClientStateToString(new_state));
    state_ = new_state;
  }


  void VerifyGameId() {
    assert(game_id_ == cli_talker_.SendMyGameIdAndGetTheOther());
  }

  void PlaceShips() {
    std::vector<ShipPlacementInfo> plan = cli_brain_.GenerateShipPlacingPlan(StrategyPlaceShip::kRandom);
    for (auto placement : plan) {
      // TODO place ship may fail, but only if the brain generated a faulty plan
      // brain should be responsible for the error-free placement plan.
      bool success = my_board_.PlaceAShip(placement.type, placement.head_location, placement.direction);
      assert(success);
    }
  }

  // this function returns when both clients are ready
  void Ready() {
    cli_talker_.SendMyReadyAndWaitTheOther();
  }

  bool DecideWhoFireFirst() {
    return cli_talker_.DecideWhoFireFirst();
  }

  // return true if win
  bool MakeOneMove() {
    // increment my move number by one
    my_board_.IncrementOneMove();

    AttackResult res = cli_talker_.Attack(cli_brain_.GenerateNextAttackLocation(StrategyAttack::kProbabilitySimple));
    cli_brain_.DigestAttackResult(res);
    if (res.attacker_win) return true;
    return false;
  }

  // return true if lose
  bool WaitForEnemyAndReplyWithResult() {
    // increment enemy move number by one
    GetRefEnemyBoard().IncrementOneMove();

    AttackResult res = my_board_.Attack(cli_talker_.GetEnemyMove());
    cli_talker_.SendAttackResult(res.success, res.sink_ship_type, res.attacker_win);
    if (res.attacker_win) return true;
    return false;
  }

  // set winner and loser on boards
  void SetWinnerLoserOnBoards(){
    my_board_.SetGameOver();
    cli_brain_.GetRefEnemyBoard().SetGameOver();
    if(is_winner_me_){
      my_board_.SetThisWinner();
    }else{
      cli_brain_.GetRefEnemyBoard().SetThisWinner();
    }
  }

  void OutputResultAndExit() {
    LogResult(cli_id_, game_id_, is_winner_me_, my_board_.GetNumMoves());

#ifdef CLEAN_EXIT
    exit(0);
#endif
  }

};


#endif //BATTLESHIP_CLIENT_GAME_CLIENT_H
