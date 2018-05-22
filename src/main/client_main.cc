//
// Created by Kong, Chuilian on 5/16/18.
//

// if CLEAN_QUIT is not defined, the game ui will never quit,
// and we main thread will never get the game result.
//#define CLEAN_EXIT

#include <thread>
#include <future>
#include "tclap/CmdLine.h"
#include "client/game_client.h"
#include "graphic/game_ui.h"

#include <pthread.h>
#include <signal.h>

// TODO: this is only for signal handler, globel
GameUi * p_ui;


void ParseArgs(const int argc, const char** argv, ClientType* type, std::string* peer_ip, size_t* port, unsigned* client_id, unsigned* game_id){
  try{
    TCLAP::CmdLine cmd("battleship game client", ' ', "1.0");

    TCLAP::ValueArg<std::string> typeArg("t", "type", "client type: initiator or listener, lower case", true, "not a type", "string");
    TCLAP::ValueArg<std::string> ipArg("a", "ip", "peer ip address", true, "127.0.0.1", "string");
    TCLAP::ValueArg<std::size_t> portArg("p", "port", "peer port", true, 0, "size_t");
    TCLAP::ValueArg<unsigned> idArg("i", "id", "client id", true, 0, "unsigned");
    TCLAP::ValueArg<unsigned> gameArg("g", "game", "game id", true, 0, "unsigned");

    cmd.add(typeArg);
    cmd.add(ipArg);
    cmd.add(portArg);
    cmd.add(idArg);
    cmd.add(gameArg);

    // Parse the argv array.
    cmd.parse(argc, argv);

    // Get the value parsed by each arg.
    *type = typeArg.getValue() == "initiator" ? ClientType::kInitiator : ClientType::kListener;
    *peer_ip = ipArg.getValue();
    *port = portArg.getValue();
    *client_id = idArg.getValue();
    *game_id = gameArg.getValue();

  } catch (TCLAP::ArgException &e){
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

}

GameStat RunClient(GameClient& cli){
  return cli.run();
}

void main_thread_signal_handler(int signal) {
  if(signal == SIGUSR1){
    // TODO need a way to get game result
    p_ui -> stop();
  }else{
    Logger("unexpected signal");
    assert(false);
  }
}

int main(const int argc, const char** argv){
  ClientType type;
  std::string peer_ip;
  size_t port;
  unsigned client_id;
  unsigned game_id;

  ParseArgs(argc, argv, &type, &peer_ip, &port, &client_id, &game_id);

  signal(SIGUSR1, main_thread_signal_handler);

  GameClient client(type, peer_ip, port, client_id, game_id, pthread_self());

  GameUi ui(client.GetRefMyBoard(), client.GetRefEnemyBoard());

  p_ui = &ui;

  std::future<GameStat> ret = std::async(&RunClient, std::ref(client));

  ui.run();

  GameStat game_stat = ret.get();

  if(game_stat.result == GameResult::kWin){
    std::cout << "win" << std::endl;
  }else if(game_stat.result == GameResult::kLose){
    std::cout << "lose" << std::endl;
  }else{
    assert(false);
  }

  std::cout << game_stat.num_moves << std::endl;

}