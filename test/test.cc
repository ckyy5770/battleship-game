#include <iostream>
#include "test_timer.h"

int main(int argc, char** argv){
  std::cout << "testing.." << std::endl;

  {
    TestTimer timer("test1");
    int sum = 0;
    for(int i=0;i<10000000;i++){
      sum += i;
    }
  }

  return 0;
}
