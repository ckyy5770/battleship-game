//
// Created by Kong, Chuilian on 5/16/18.
//

// if CLEAN_QUIT is not defined, the game ui will never quit,
// and we main thread will never get the game result.
#define CLEAN_EXIT

#include <thread>
#include "tclap/CmdLine.h"
#include "client/game_client.h"
#include "graphic/game_ui.h"

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

void RunClient(GameClient& cli){
  cli.run();
  return;
}

int main(const int argc, const char** argv){
  ClientType type;
  std::string peer_ip;
  size_t port;
  unsigned client_id;
  unsigned game_id;

  ParseArgs(argc, argv, &type, &peer_ip, &port, &client_id, &game_id);

  GameClient client(type, peer_ip, port, client_id, game_id, pthread_self());

  GameUi ui(client.GetRefMyBoard(), client.GetRefEnemyBoard());

  std::thread cli_thread(RunClient, std::ref(client));

  ui.run();

}