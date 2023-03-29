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
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif



// Sequential lcs algorithm
void sequenial_lcs(int m, int n, int** C, char *X, char *Y) {
  // DEBUG
  // std::cout << "in sequential_lcs" << std::endl;
  /**/
  for (int nd = 1; nd <= (m + n - 1); ++nd) {
    int x, y, beg, end;
    // DEBUG
    // std::cout << "~~~~~~~~~~ >> grain #" << nd << std::endl;
    if (nd <= std::max(m, n)) {
      // DEBUG
      // std::cout << "~~~~~~~~~~ >> IF" << std::endl;
      beg = 0;
      end = nd - 1;
    } else {
      // DEBUG
      // std::cout << "~~~~~~~~~~ >> ELSE" << std::endl;
      beg = nd - (std::max(m, n));
      end = std::max(m, n) - 1;
    }
    
    for (int i = beg; i <= end; ++i) {
      x = i; if (x >= m - 1) {x = m - 1;} // -- x = a - 1;
      y = end - i + beg; if (y >= n - 1) {y = n - 1;} // -- y = b - 1;
      // DEBUG
      // std::cout << "--> X[" << x << "], Y[" << y << "]: " << X[x] << ", " << Y[y] << std::endl;
      // std::cout << "===>> x: " << x << " <<==="<< std::endl;
      // std::cout << "===>> y: " <<  y << " <<==="<< std::endl;
      if (X[x] == Y[y]) {
        // DEBUG
        // std::cout << "!!! MATCH !!!"<<std::endl;
        // std::cout << "---> C[" << x << "+1][" << y << "+1] = " << C[x+1][y+1] << std::endl;
        C[x+1][y+1] = C[x][y] + 1;
      } else {
        C[x+1][y+1] = std::max(C[x][y+1], C[x+1][y]);
      }
      // DEBUG
      // std::cout << "---> C[" << x << "][" << y << "] = " << C[x][y] << std::endl;
    }
  }
  /**/
  // DEBUG
  // std::cout << "exiting sequential_lcs" << std::endl;
};



int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  // OmpLoop object declaration
  OmpLoop omploop;

  // Mutex declaration
  std::mutex mtx;

  // Declaring int array C
  int** C;
  int result, beg, end;

  // Gathering arguments
  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  // Parloop function declaractions
  std::function<void(int)> f = [&](int i) {
    // DEBUG
    // std::cout << "in f: " << std::endl;
    // std::cout << "i: " << (i + 1) << std::endl;
    /**/
    int x, y;
    x = i; if (x >= m - 1) {x = m - 1;} // -- x = a - 1;
    y = end; if (y >= n - 1) {y = n - 1;} // -- y = b - 1;
    // DEBUG
    // std::cout << "===>> x: " << x << " <<==="<< std::endl;
    // std::cout << "===>> y: " << y << " <<==="<< std::endl;
    // std::cout << "--> X[" << x << "], Y[" << y << "]: " << X[x] << ", " << Y[y] << std::endl;
    if (X[x] == Y[y]) {
      // DEBUG
      // std::cout << "!!! MATCH !!!"<<std::endl;
      // std::cout << "---> C[" << x << "+1][" << y << "+1] = " << C[x+1][y+1] << std::endl;
      C[x+1][y+1] = C[x][y] + 1; 
    } else {
      C[x+1][y+1] = std::max(C[x][y+1], C[x+1][y]);
    }
    // DEBUG
    // std::cout << "---> C[" << x << "][" << y << "] = " << C[x][y] << std::endl;
    /**/
    // DEBUG
    // std::cout << "exiting f" << std::endl;
  };

  /// TODO: insert LCS code here
  /// COMPLETED: insert LCS code here
  // Time start
  std::chrono::time_point<std::chrono::system_clock> time_start = std::chrono::system_clock::now();

  /// TODO: Write and run LCS algorithm in parallel
  /// COMPLETED: Write and run LCS algorithm in parallel
  C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }
  omploop.setNbThread(nbthreads); // -- omploop.setGranularity(m + n - 1);
  /**/
  for (int nd = 1; nd <= (m + n - 1); ++nd) {
    if (nd <= std::max(m, n)) {
      beg = 0;
      end = nd;
    } else {
      beg = nd - (std::max(m, n));
      end = std::max(m, n);
    }
    // DEBUG
    // std::cout << "\n\n\n~~~~~~~~~~\n~~~~~~~~~~\n~~~~~~~~~~ >>" << std::endl;
    // std::cout << "-> beg, end: " << beg << ", " << end - 1 << std::endl;
    omploop.parfor(beg, end, 1,
      f
    );
  }
  /**/

  // DEBUG
  // std::cout << "X: " << X << std::endl;
  // std::cout << "Y: " << Y << std::endl;
  // std::cout << "length of X & Y: " << m << ", " << n << std::endl;
  // std::cout << "num diagonals: " << m + n - 1 << std::endl;

  /// TODO: Write and run sequential LCS algorithm
  /// COMPLETED: Write and run sequential LCS algorithm
  // sequenial_lcs(m, n, C, X, Y);


  result = C[m][n]; // length of common subsequence
  // DEBUG
  // std::cout << "[][][] >>> result: " << result << std::endl;
  // std::cout << "[][][] >>> C[" << m << "][" << n << "]: " << C[m][n] << std::endl;

  // Time end
  std::chrono::time_point<std::chrono::system_clock> time_end = std::chrono::system_clock::now();

  // Time elapsed
  std::chrono::duration<double> elapsed_seconds = time_end-time_start;

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
