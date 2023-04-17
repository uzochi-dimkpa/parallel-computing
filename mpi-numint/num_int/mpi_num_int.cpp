#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

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
  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  
  MPI_Init (&argc, &argv); int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size); MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // double total [1];
  double total;
  
  // DEBUG
  // std::cout << "I am process " << rank+1 << " out of " << size << std::endl;
  
  // Integration function parameters
  int fid = atoi(argv[1]);
  double a = atof(argv[2]); double b = atof(argv[3]); int64_t n = (int64_t) atoi(argv[4]);
  int intensity = atoi(argv[5]);
  double coeff = (double) ((b - a) / n);
  // double out [1]; out[0] = 0.0;
  double out;
  typedef float (*fnct[4]) (float, int); double x; int i;
  
  // Function selection
  fnct function = {&f1, &f2, &f3, &f4};
  
  // Integration math & timer
  auto timeStart = std::chrono::system_clock::now();
  
  for (i = rank * (n / size); i < ((rank + 1) * (n / size)); i++) {
    x = (a + (coeff * (i + 0.5)));
    out += function[fid - 1](x, intensity);
    // out += (function[fid - 1](x, intensity)) * coeff;
  }
  out *= coeff;
  total += out;
  
  auto timeEnd = std::chrono::system_clock::now();
  std::chrono::duration <double> duration = timeEnd - timeStart;
  
  // DEBUG
  /*
  std::cout << "BEFORE" << std::endl;
  std::cout << "\ttotal:\t" << total << std::endl;
  std::cout << "\tout:\t" << out << std::endl << std::endl;
  /**/
  
  // MPI_Bcast(&out, size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Reduce(&(out), &(total), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  // Output
  /**/
  if (rank == 0) {
    std::cout << total << std::endl; // answer
    std::cerr << duration.count() << std::endl; // time
  }
  /**/
  
  // DEBUG
  /*
  std::cout << "\ttotal:\t" << total << std::endl;
  std::cout << "\tout:\t" << out << std::endl << std::endl;
  /**/
  
  MPI_Finalize();
  
  return 0;
}
