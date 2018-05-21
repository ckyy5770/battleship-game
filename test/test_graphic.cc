//
// Created by Kong, Chuilian on 5/20/18.
//

#include <thread>
#include <iostream>
#include "graphic/game_ui.h"

void attack_my_board(Board & my_board){
  for(size_t i = 0; i < 100; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    my_board.Attack(i);
  }
}

void attack_enemy_board(ImagineBoard & enemy_board){
  bool toggle = true;
  for(size_t i = 0; i < 100; i++){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(toggle) enemy_board.MarkAttack(i);
    else enemy_board.MarkOccupied(i);
    toggle = !toggle;
  }
}

void place_ship(Board& my_board){
  my_board.PlaceAShip(ShipType::kBattleShip, 0, Direction::kVertical);
  my_board.PlaceAShip(ShipType::kCruiser, 3, Direction::kHorisontal);

}

int main(void)
{
  Board my_board;
  ImagineBoard enemy_board;
  GameUi ui(my_board, enemy_board);

  std::thread thread1(attack_my_board, std::ref(my_board));
  std::thread thread2(attack_enemy_board, std::ref(enemy_board));
  std::thread thread3(place_ship, std::ref(my_board));

  ui.run();
  return 0;
}