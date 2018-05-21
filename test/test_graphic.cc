//
// Created by Kong, Chuilian on 5/20/18.
//

#include <thread>
#include <iostream>
#include "graphic/game_ui.h"

void attack(Board & my_board){
  for(size_t i = 0; i < 100; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    my_board.Attack(i);
    std::cout << "attacked " << i << std::endl;
  }
}

int main(void)
{
  Board my_board;
  ImagineBoard enemy_board;
  GameUi ui(my_board, enemy_board);

  std::thread new_thread(attack, std::ref(my_board));

  ui.Run();
  return 0;
}