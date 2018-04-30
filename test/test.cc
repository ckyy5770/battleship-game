#include <iostream>
#include <algorithm>
#include "test_timer.h"

// Test access speed of 1-d array and 2-d arry.
// Note: we give x-y index so accessing 1-d array involve manually calculate the index.
// result: sometimes 1-D array faster and sometimes 2-D array faster, no significant difference

// test_array_1d_2d
// 1-D array completed in 0.00264944s.
// 2-D array completed in 0.00266808s.

// test_array_1d_2d
// 1-D array completed in 0.00265523s.
// 2-D array completed in 0.00265704s.

// test_array_1d_2d
// 1-D array completed in 0.00271311s.
// 2-D array completed in 0.00268861s.

void test_array_1d_2d(){
  std::cout << "test_array_1d_2d" << std::endl;

  const std::size_t DIM = 100;
  const std::size_t ACCESS_TIMES = DIM * DIM * 100;
  // random coordinates
  int arr_x[ACCESS_TIMES];
  for(int i = 0; i < ACCESS_TIMES; i++){
    arr_x[i] = i % DIM;
  }
  int arr_y[ACCESS_TIMES];
  for(int i = 0; i < ACCESS_TIMES; i++){
    arr_y[i] = i % DIM;
  }

  std::random_shuffle(arr_x, arr_x + ACCESS_TIMES);
  std::random_shuffle(arr_y, arr_y + ACCESS_TIMES);

  // construct 1-D array
  int arr_1[DIM * DIM];
  for(int i = 0; i<DIM * DIM; i++){
    arr_1[i] = i;
  }

  // construct 2-D array
  int arr_2[DIM][DIM];
  for(int i = 0; i<DIM * DIM; i++){
    arr_2[i / DIM][i % DIM] = i;
  }

  {
    TestTimer timer("1-D array");
    long sum = 0;
    for(int i=0; i<ACCESS_TIMES; i++){
      sum += arr_1[arr_x[i] * DIM + arr_y[i]];
    }
  }

  {
    TestTimer timer("2-D array");
    long sum = 0;
    for(int i=0; i<ACCESS_TIMES; i++){
      sum += arr_2[arr_x[i]][arr_y[i]];
    }
  }
}


int main(int argc, char** argv){
  test_array_1d_2d();

  return 0;
}
