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
  
  // Dynamic object declaration
  DynamicLoop dynamicloop;
  
  // Integration function parameters
  int fid = atoi(argv[1]); int nbthreads = atoi(argv[6]); int granularity = atoi(argv[7]);
  double a = atof(argv[2]); double b = atof(argv[3]); int64_t n = (int64_t) atoi(argv[4]);
  int intensity = atoi(argv[5]);
  double coeff = (double) ((b - a) / n);
  double total = 0.0;
  int chunk_sizes[2];
  typedef float (*fnct[4]) (float, int); double x; int i;
  
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
    out += function[fid - 1]((a + (coeff * (i + 0.5))), intensity) * coeff;
    // DEBUG
    // std::cout << "x: " << (a + (coeff * (i + 0.5))) << ", out: " << out << std::endl;
    // std::cout << "exiting f" << std::endl;
  };
  
  std::function<void(double&)> after = [&](double& out) {
    // DEBUG
    // std::cout << "in after" << std::endl;
    total += out;
    // DEBUG
    // std::cout << "total: " << total << std::endl;
    // std::cout << "exiting after" << std::endl;
  };
  
  dynamicloop.num_threads = nbthreads; dynamicloop.granularity = granularity;
  
  dynamicloop.parfor<double>(0, n, 1,
    before
    ,
    f
    ,
    after
  );
  
  // Original integration
  // for (i = 0; i < n; i++) {
  //   x = (a + (coeff * (i + 0.5))); out += function[fid - 1](x, intensity) * coeff;
  //   out += function[fid - 1]((a + (coeff * (i + 0.5))), intensity) * coeff;
  // }
  
  // Time end
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  
  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = end-start;
  
  // Print integration output
  std::cout<<total<<std::endl;
  
  // // Original computation start -- for calculation accuracy check
  // double x_original = 0.0; double out_original = 0.0;
  // staticloop.parfor(0, n, 1,
  //   [&](int i) -> void {
  //     // --
  //     x_original = (a + (coeff * (i + 0.5)));
  //     out_original += function[fid - 1](x_original, intensity) * coeff;
  //   }
  // );
  // // Original computation end
  // // DEBUG
  // std::cout << "\n~~~~~~~~~~" << std::endl;
  // std::cout << "n: " << n << std::endl;
  // std::cout << "Correct answer: " << out_original << "\n~~~~~~~~~~\n" << std::endl;
  
  // Print time output
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
