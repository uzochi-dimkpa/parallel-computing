#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_loop.cpp"

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }
  
  // Time start
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  // --
  for (i = 0; i < n; i++) {
    x = (a + (coeff * (i + 0.5))); out += function[fid - 1](x, intensity) * coeff;
    out += function[fid - 1]((a + (coeff * (i + 0.5))), intensity) * coeff;
  }
  
  // Time end
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  
  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = end-start;
  
  // Print integration output
  std::cout<<total<<std::endl;
  
  // Print time output
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
