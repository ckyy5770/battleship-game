// Created by Chuilian Kong April 30, 2018

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <iostream>
#include <string>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG

static void Logger(const std::string & what){
  std::cout << what << std::endl;
}

static void LogResult(ClientId cli_id, GameId game_id, bool does_win, size_t num_moves){
  std::cout << cli_id << ","
            << game_id << ","
            << (does_win ? "win" : "lose") << ","
            << num_moves << std::endl;

}
#endif  // UTILS_UTILS_H_
