// #ifndef __OMPLOOP__
// #define __OMPLOOP__

// #include <functional>

// #include <vector>
// #include <thread>
// #include <mutex>

// // That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// // You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// // set number of thread by called setNbThread.
// class OmpLoop {

// int nbthread, M, N;

// public:

//   int granularity, grain_count = 0;

//   OmpLoop()
//     :nbthread(1), granularity(1)
//   {}
  
//   void setNbThread(int t) {
//     nbthread = t;
//   }

//   void setGranularity(int g) {
//     granularity = g;
//   }

//   void setM(int m) {
//     M = m;
//   }

//   void setN(int n) {
//     N = n;
//   }

//   // Mutex declaration
//   std::mutex mtx;

//   // Declaring vector of std::thread type
//   std::vector<std::thread> threadbatch;

//   template<typename TLS>
//   void parfor (size_t beg, size_t end, size_t increment,
//                 std::function<void(TLS&)> before, // -- TLS&
//                 std::function<void(int, TLS&)> f, // -- TLS&
//                 std::function<void(TLS&)> after // -- TLS&
//                 ) {
//   #pragma omp parallel num_threads(nbthread)
//       {
//         TLS tls;
//         before(tls);
        
//   #pragma omp for schedule(dynamic, granularity)
//         for (size_t i=beg; i<end; i+= increment) {
//           f(i, tls);
//         }
        
//   #pragma omp critical
//         after(tls);
//       }
    
//     // Parallel running threads
//     // for (int t = 0; t < nbthread; ++t) {
//     //   threadbatch.push_back (
//     //     std::move (
//     //       std::thread (
//     //         [&]() {
//     //           f();
//     //         }
//     //       )
//     //     )
//     //   );
//     // }
    
//     // Parallel joining threads
//     for (auto &t: threadbatch) {
//       if (t.joinable()) {
//         t.join();
//       }
//     }
  
//   }
// };

// #endif

#ifndef __OMPLOOP__
#define __OMPLOOP__

#include <functional>

// That does not solve the static activity, but provide the same feature so it can be used as a replacement.
// You need to add -fopenmp to CXXFLAGS and LDFLAGS for this to work.
// set number of thread by called setNbThread.
class OmpLoop {

  int nbthread;
  int granularity;

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


  void parfor (size_t beg, size_t end, size_t increment,
                std::function<void(int)> f
                ) {
#pragma omp parallel num_threads(nbthread)
    {
#pragma omp for schedule(dynamic, granularity) 
      for (size_t i=beg; i<end; i+= increment) {
        f(i);
      }
    }
  }

  
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
                std::function<void(TLS&)> before,
                std::function<void(int, TLS&)> f,
                std::function<void(TLS&)> after
                ) {
#pragma omp parallel num_threads(nbthread)
    {
      TLS tls;
      before(tls);
      
#pragma omp for schedule(dynamic, granularity) 
      for (size_t i=beg; i<end; i+= increment) {
	f(i, tls);
      }
#pragma omp critical
      after(tls);
    }
  }

};

#endif