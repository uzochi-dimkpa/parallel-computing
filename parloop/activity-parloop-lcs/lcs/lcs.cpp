#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "omploop.hpp"
#include <chrono>

#include <vector>
#include <thread>

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  // OmpLoop object declaration
  OmpLoop omploop;

  // Declaring vector of std::thread type
  std::vector<std::thread> threadbatch;

  // Declaring int array C
  int** C;
  int result, num_diagonals;

  // Gathering arguments
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  // Parloop function declaractions
  std::function<void()> before = [&]() { // -- double& out
    // DEBUG
    // std::cout << "in before" << std::endl;
    // -- 
    // DEBUG
    // std::cout << "exiting before" << std::endl;
  };
  
  std::function<void()> f = [&]() {
    // DEBUG
    // std::cout << "in f: " << std::endl;
    // std::cout << "i: " << (i + 1) << std::endl;
    // -- //
    for (int nd = 1; nd <= num_diagonals; ++nd) {
      int xmax, ymax;
      
      if (nd <= std::max(m, n)) {
        int beg = 1;
        int end = nd;
        int a = 1;
        int b = nd;
        
        for (int i = beg; i <= end; ++i) {
          xmax = a - 1; if ((a-1) >= m) {xmax = m - 1;}
          ymax = b - 1; if ((b-1) >= n) {ymax = n - 1;}
          
          if (X[xmax] == Y[ymax]) {
            C[xmax+1][ymax+1] = C[xmax][ymax] + 1;
          } else {
            C[xmax+1][ymax+1] = std::max(C[xmax][ymax+1], C[xmax+1][ymax]);
          }
          
          if (i == end) {
            break;
          } else{
            ++a; --b;
          }
        }
      } else {
        int beg = nd - (std::max(m, n)) + 1;
        int end = std::max(m, n);
        int a = nd - (std::max(m, n)) + 1;
        int b = std::max(m, n);
        
        for (int i = beg; i <= end; ++i) {
          xmax = a - 1; if ((a-1) >= m) {xmax = m - 1;}
          ymax = b - 1; if ((b-1) >= n) {ymax = n - 1;}
          
          if (X[xmax] == Y[ymax]) {
            C[xmax+1][ymax+1] = C[xmax][ymax] + 1; 
          } else {
            C[xmax+1][ymax+1] = std::max(C[xmax][ymax+1], C[xmax+1][ymax]);
          }
          
          if (i == end) {
            break;
          } else{
            ++a; --b;
          }
        }
      }
    }
    // -- //
    // DEBUG
    // std::cout << "exiting f" << std::endl;
  };
  
  std::function<void()> after = [&]() {
    // DEBUG
    // std::cout << "in after" << std::endl;
    // --
    // DEBUG
    // std::cout << "exiting after" << std::endl;
  };

  /// TODO: insert LCS code here.
  /// COMPLETED: insert LCS code here.
  // Time start
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  /// TODO: Parallel LCS algorithm
  /// COMPLETED: Parallel LCS algorithm
  C = new int*[m+1]; num_diagonals = m + n - 1;
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }

  // Running algorithm
  omploop.setM(m); omploop.setN(n);
  omploop.setNbThread(nbthreads); omploop.setGranularity(m + n - 1);
  omploop.parfor<double>(0, nbthreads, 1,
    before
    ,
    f
    ,
    after
  );

  // DEBUG
  // std::cout << "X: " << X << std::endl;
  // std::cout << "Y: " << Y << std::endl;
  // std::cout << "length of X & Y: " << m << ", " << n << std::endl;
  // std::cout << "num threads: " << m + n - 1 << std::endl;
  // DEBUG
  // std::cout << "========== X[" << 1 << "] & Y[" << 4 << "] ----> " << X[1] << " & " << Y[4] << std::endl;

  /// TODO: Write Paralle LCS algorithm
  /// COMPLETED: Write Paralle LCS algorithm
  // for (int nd = 1; nd <= num_diagonals; ++nd) {
  //   int xmax, ymax;
  //   // DEBUG
  //   // std::cout << "~~~~~~~~~~ >> thread #" << nd << std::endl;
    
  //   if (nd <= std::max(m, n)) {
  //     int beg = 1;
  //     int end = nd;
  //     int a = 1;
  //     int b = nd;
      
  //     for (int i = beg; i <= end; ++i) {
  //       // DEBUG
  //       xmax = a - 1; if ((a-1) >= m) {xmax = m - 1;}
  //       ymax = b - 1; if ((b-1) >= n) {ymax = n - 1;}
  //       // std::cout << "====> a, b: " << a << ", " << b << std::endl;
  //       // std::cout << "-> xmax: " << xmax << std::endl;
  //       // std::cout << "-> ymax: " << ymax << std::endl;
  //       // std::cout << "--> X[" << xmax << "], Y[" << ymax << "]: " << X[xmax] << ", " << Y[ymax] << std::endl;
  //       // std::cout << xmax << ", " << typeid(X).name() << std::endl;
  //       // int xmax = sizeof(X) / sizeof(X[0]); int ymax = sizeof(Y) / sizeof(Y[0]);
  //       // if ((a-1) >= (sizeof(X) / sizeof(X[0])) {}
  //       if (X[xmax] == Y[ymax]) {
  //         // DEBUG
  //         // std::cout << "!!! MATCH !!!"<<std::endl;
  //         C[xmax+1][ymax+1] = C[xmax][ymax] + 1;
  //       } else {
  //         C[xmax+1][ymax+1] = std::max(C[xmax][ymax+1], C[xmax+1][ymax]);
  //       }
  //       // DEBUG
  //       // std::cout << "---> C[" << xmax << "][" << ymax << "] = " << C[xmax][ymax] << std::endl;
  //       if (i == end) {
  //         break;
  //       } else{
  //         ++a; --b;
  //       }
  //     }
  //   } else {
  //     int beg = nd - (std::max(m, n)) + 1;
  //     int end = std::max(m, n);
  //     int a = nd - (std::max(m, n)) + 1;
  //     int b = std::max(m, n);
      
  //     for (int i = beg; i <= end; ++i) {
  //       xmax = a - 1; if ((a-1) >= m) {xmax = m - 1;}
  //       ymax = b - 1; if ((b-1) >= n) {ymax = n - 1;}
  //       // DEBUG
  //       // std::cout << "====> a, b: " << a << ", " << b << std::endl;
  //       // std::cout << "-> xmax: " << xmax << std::endl;
  //       // std::cout << "-> ymax: " << ymax << std::endl;
  //       // std::cout << "--> X[" << xmax << "], Y[" << ymax << "]: " << X[xmax] << ", " << Y[ymax] << std::endl;
  //       if (X[xmax] == Y[ymax]) {
  //         // DEBUG
  //         // std::cout << "!!! MATCH !!!"<<std::endl;
  //         C[xmax+1][ymax+1] = C[xmax][ymax] + 1; 
  //       } else {
  //         C[xmax+1][ymax+1] = std::max(C[xmax][ymax+1], C[xmax+1][ymax]);
  //       }
  //       // DEBUG
  //       // std::cout << "---> C[" << xmax << "][" << ymax << "] = " << C[xmax][ymax] << std::endl;
  //       if (i == end) {
  //         break;
  //       } else{
  //         ++a; --b;
  //       }
  //     }
  //   }
  // }

  result = C[m][n]; // length of common subsequence
  // DEBUG
  // std::cout << "[][][] >>> result: " << result << std::endl;

  // Time end
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = end-start;

  // Print LCS output
  checkLCS(X, m, Y, n, result);
  // std::cout<<result<<std::endl;

  // Print time output
  std::cerr<<elapsed_seconds.count()<<std::endl;

  // Memory deallocation
  delete[] X;
  delete[] Y;

  for (int i=0; i<=m; ++i) { 
    delete[] C[i];
  }
  delete[] C;

  // Original sequential LCS algorithm
  // C = new int*[m+1];
  // for (int i=0; i<=m; ++i) {
  //   C[i] = new int[n+1];
  //   C[i][0] = 0;
  // }
  // for (int j=0; j<=n; ++j) {
  //   C[0][j] = 0;
  // }
  // for (int a=1; a<=m; ++a) {
  //   for (int b=1; b<=n; ++b) {
  //     if (X[a-1] == Y[b-1]) {
  //       C[a][b] = C[a-1][b-1] + 1; 
  //     } else {
  //       C[a][b] = std::max(C[a-1][b], C[a][b-1]);
  //     }
  //   }
  // }
  // result = C[m][n]; // length of common subsequence

  return 0;
}
