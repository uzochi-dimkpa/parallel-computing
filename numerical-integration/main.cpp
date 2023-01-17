#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

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

// argc: argument count
// argv: argument values
int main (int argc, char* argv[]) {
  // Error handling
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  // Integration function parameters
  int fid = atoi(argv[1]);
  double a = atof(argv[2]); double b = atof(argv[3]); int64_t n = (int64_t) atoi(argv[4]);
  int intensity = atoi(argv[5]);
  double coeff = (double) ((b - a) / n);
  double out = 0.0;
  typedef float (*fnct[4]) (float, int); double x; int i;

  // Function selection
  fnct function = {&f1, &f2, &f3, &f4};

  // DEBUG
  // Argument count/readout
  /*
  if (argc >= 1) {
    std::cout << "Your inputs:\n~~~\nfunction id: " << fid << "\na: " << a << "\nb: " << b << "\nn: " << n << "\nintensity: " << intensity << "\ncoeff: "<< coeff << std::endl;
    std::cout << "function address: " << &function[fid] << std::endl;
  } else {
    std::cout << "Your inputs: " << "empty!" << std::endl;
  }
  /**/

  // Integration math & timer
  auto timeStart = std::chrono::system_clock::now();

  for (i = 0; i < n; i++) {
    x = (a + (coeff * (i + 0.5)));

    // DEBUG
    // x parameter readout
    //...std::cout << "x" << i + 1 << ": " << x << std::endl;

    out += function[fid - 1](x, intensity);
  }
  out *= coeff;

  auto timeEnd = std::chrono::system_clock::now();
  std::chrono::duration duration = timeEnd - timeStart;

  // Output
  std::cout << out << std::endl; // answer
  std::cerr << duration.count() << std::endl; // time

  return 0;
}
