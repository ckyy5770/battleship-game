//
// Created by Kong, Chuilian on 5/21/18.
//

#ifndef BATTLESHIP_GAME_RANDOM_UNIT_H
#define BATTLESHIP_GAME_RANDOM_UNIT_H

#include <algorithm>
#include <random>
#include <ctime>
#include <memory>

// please use a unique pointer outside this function to handle new allocated memory
class RandomUnit{
public:

  static size_t* NewRandomSequenceArray(size_t dim){
    std::unique_ptr<size_t[]> up_new_rand_seq(new size_t[dim]);
    for(size_t i = 0; i < dim; i++){
      up_new_rand_seq[i] = i;
    }
    std::random_device rd;
    std::mt19937 engine(rd());
    std::shuffle(up_new_rand_seq.get(), up_new_rand_seq.get() + dim, engine);
    return up_new_rand_seq.release();
  }

  static Direction* NewRandomDirectionArray(size_t size){
    std::unique_ptr<Direction[]> up_new_rand_dir(new Direction[size]);
    bool toggle = true;
    for(size_t i = 0; i < size; i++){
      up_new_rand_dir[i] = toggle ? Direction::kHorisontal : Direction::kVertical;
      toggle = !toggle;
    }
    std::random_device rd;
    std::mt19937 engine(rd());
    std::shuffle(up_new_rand_dir.get(), up_new_rand_dir.get() + size, engine);
    return up_new_rand_dir.release();
  }

  static size_t GetRandomSizeT(size_t inclusive_low, size_t inclusive_hi){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> dist(inclusive_low, inclusive_hi);
    return dist(mt);
  }
};

#endif //BATTLESHIP_GAME_RANDOM_UNIT_H
