//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_RANDOM_UNIT_H
#define BATTLESHIP_GAME_RANDOM_UNIT_H

#include <algorithm>
#include <random>
#include <ctime>
#include <memory>

class RandomUnit{
public:
  // please use a unique pointer outside this function to handle new allocated memory
  static size_t* NewRandomSequence(size_t dim){
    std::unique_ptr<size_t[]> up_new_rand_seq(new size_t[dim]);
    for(size_t i = 0; i < dim; i++){
      up_new_rand_seq[i] = i;
    }
    std::shuffle(up_new_rand_seq.get(), up_new_rand_seq.get() + dim, std::default_random_engine(time(nullptr)));
    return up_new_rand_seq.release();
  }
};

#endif //BATTLESHIP_GAME_RANDOM_UNIT_H
