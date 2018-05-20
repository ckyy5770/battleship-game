//
// Created by Kong, Chuilian on 5/20/18.
//

#ifndef BATTLESHIP_GAME_BASE_BOARD_H
#define BATTLESHIP_GAME_BASE_BOARD_H

#include <iostream>
#include "core/game/game_common.h"


class BaseBoard{
public:
  static void RenderBaseBoard(){
    for(size_t i = 0; i < kDim; i++){
      for(size_t j = 0; j < kDim; j++) {
        std::cout << "+------";
      }
      std::cout << std::endl;
      for(size_t j = 0; j < kDim; j++) {
        std::cout << "|      ";
      }
      std::cout << std::endl;
      for(size_t j = 0; j < kDim; j++) {
        std::cout << "|      ";
      }
      std::cout << std::endl;
      for(size_t j = 0; j < kDim; j++) {
        std::cout << "|      ";
      }
      std::cout << std::endl;
    }
  }
private:
};

#endif //BATTLESHIP_GAME_BASE_BOARD_H
