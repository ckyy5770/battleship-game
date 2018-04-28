// Created by Chuilian Kong Apr 27, 2018
// High resolution timer for benchmarking

#include <iostream>
#include <string>
#include <chrono>

class TestTimer{
public:
  TestTimer(const std::string & name):
    name_(name),
    start_(std::chrono::high_resolution_clock::now())
  {}

  ~TestTimer(){
    auto time_elapsed = std::chrono::high_resolution_clock::now() - start_;
    std::cout << name_
      << " completed in "
      << std::chrono::duration_cast<std::chrono::duration<double>>(time_elapsed).count()
      << "s."
      << std::endl;
  }
private:
  const std::string & name_;
  const std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
