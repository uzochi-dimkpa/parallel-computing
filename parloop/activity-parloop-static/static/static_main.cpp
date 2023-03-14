#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include "static_loop.cpp"


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


// --
/*
double integrate (*fnct fid, float a, float b, int64_t n, int intensity, double coeff) {
  return out
}
/**/

int main (int argc, char* argv[]) {
  
  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  
  // Parallel for loop file
  // --
  StaticLoop staticloop;
  
  // Integration function parameters
  int fid = atoi(argv[1]);
  double a = atof(argv[2]); double b = atof(argv[3]); int64_t n = (int64_t) atoi(argv[4]);
  int intensity = atoi(argv[5]);
  double coeff = (double) ((b - a) / n);
  double out = 0.0;
  typedef float (*fnct[4]) (float, int); double x; int i;
  
  // Function selection
  fnct function = {&f1, &f2, &f3, &f4};
  
  // Time etart
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  // Computation start
  staticloop.parfor(0, n, 1,
    [&](int i) -> void{
      // --
      x = (a + (coeff * (i + 0.5)));
      out += function[fid - 1](x, intensity) * coeff;
      // int y = 3;
    }
  );
  
  // for (i = 0; i < n; i++) {
  //   x = (a + (coeff * (i + 0.5)));
  //   out += function[fid - 1](x, intensity);
  // }
  // out *= coeff;
  // Computation end
  
  // Time end
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  
  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = end-start;
  
  // Print integration output
  std::cout<<out<<std::endl;
  
  // Print time output
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
