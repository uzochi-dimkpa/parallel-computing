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


int main (int argc, char* argv[]) {
  
  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  
  // StaticLoop object declaration
  StaticLoop staticloop;
  
  // DEBUG
  // std::cout << "In main" << std::endl;
  
  // Integration function parameters
  int fid = atoi(argv[1]); int nbthreads = atoi(argv[6]);
  double a = atof(argv[2]); double b = atof(argv[3]); int64_t n = (int64_t) atoi(argv[4]);
  int intensity = atoi(argv[5]);
  double coeff = (double) ((b - a) / n);
  double total = 0.0;
  int chunk_sizes[2];
  typedef float (*fnct[4]) (float, int); double x; int i;
  
  // DEBUG
  // std::cout << "Gathered arguments" << std::endl;
  
  // Function selection
  fnct function = {&f1, &f2, &f3, &f4};
  
  // Parfor function declaractions
  std::function<void(double&)> before = [&](double& out) {
    // DEBUG
    // std::cout << "in before" << std::endl;
    out = 0.0;
    // DEBUG
    // std::cout << "exiting before" << std::endl;
  };
  
  std::function<void(int, double&)> f = [&](int i, double& out) {
    // DEBUG
    // std::cout << "in f: " << std::endl;
    // std::cout << "i: " << (i + 1) << std::endl;
    // x = (a + (coeff * (i + 0.5))); out += function[fid - 1](x, intensity) * coeff;
    out += function[fid - 1]((a + (coeff * (i + 0.5))), intensity) * coeff;
    // DEBUG
    // std::cout << "x: " << (a + (coeff * (i + 0.5))) << ", out: " << out << std::endl;
    // std::cout << "exiting f" << std::endl;
  };
  
  std::function<void(double&)> after = [&](double& out) {
    // DEBUG
    // std::cout << "in after" << std::endl;
    // mtx.lock();
    total += out;
    // mtx.unlock();
    // DEBUG
    // std::cout << "total: " << total << std::endl;
    // std::cout << "exiting after" << std::endl;
  };
  
  // DEBUG
  // std::cout << "Time start" << std::endl;
  
  // Time start
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  // DEBUG
  // std::cout << "n: " << n << "\nnum threads: " << nbthreads << "\nmain chunk size: " << temp_out[0] << "\nremainder size: " << temp_out[1] << std::endl;
  
  // Computation start
  // std::cout << "Computation start" << std::endl;
  staticloop.num_threads = nbthreads;
  
  staticloop.parfor<double>(0, n, 1,
    before
    ,
    f
    ,
    after
  );
  // DEBUG
  // std::cout << "Computation end" << std::endl;
  // Computation end
  
  // Original computation start -- for calculation accuracy check
  // double x_original = 0.0; double out_original = 0.0;
  // staticloop.parfor(0, n, 1,
  //   [&](int i) -> void {
  //     // --
  //     x_original = (a + (coeff * (i + 0.5)));
  //     out_original += function[fid - 1](x_original, intensity) * coeff;
  //   }
  // );
  // Original computation endComputation end
  
  // Time end
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  
  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = end-start;
  
  // Print integration output
  std::cout<<total<<std::endl;
  // DEBUG
  // std::cout << "\n~~~~~~~~~~" << std::endl;
  // std::cout << "n: " << n << std::endl;
  // std::cout << "Correct answer: " << out_original << "\n~~~~~~~~~~\n" << std::endl;
  
  // Print time output
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
