#ifndef __OMPLOOP__
#define __OMPLOOP__

#include <functional>

#include <vector>
#include <thread>
#include <mutex>

// That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// set number of thread by called setNbThread.
class OmpLoop {

int nbthread, M, N;
int granularity, grain_count = 0;

public:

  OmpLoop()
    :nbthread(1), granularity(1)
  {}
  
  void setNbThread(int t) {
    nbthread = t;
  }

  void setGranularity(int g) {
    granularity = g;
  }

  void setM(int m) {
    M = m;
  }

  void setN(int n) {
    N = n;
  }

  // Mutex declaration
  std::mutex mtx;

  // Declaring vector of std::thread type
  std::vector<std::thread> threadbatch;

  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
                std::function<void()> before, // -- TLS&
                std::function<void()> f, // -- TLS&
                std::function<void()> after // -- TLS&
                ) {
  // #pragma omp parallel num_threads(nbthread)
  //     {
  //       // TLS tls;
  //       before();
        
  // #pragma omp for schedule(dynamic, granularity)
  //       for (size_t i=beg; i<end; i+= increment) {
  //         f();
  //       }
        
  // #pragma omp critical
  //       after();
  //     }
    
    // Parallel running threads
    while (grain_count < granularity) {
      mtx.lock(); ++grain_count; mtx.unlock();
      threadbatch.push_back (
        std::move (
          std::thread (
            [&]() { // -- , M, N
              f();
            }
          )
        )
      );
    }
    
    // Parallel joining threads
    for (auto &t: threadbatch) {
      if (t.joinable()) {
        t.join();
      }
    }
  
  }
};

#endif
