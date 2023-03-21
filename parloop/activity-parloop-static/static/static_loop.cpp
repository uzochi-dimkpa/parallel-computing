#ifndef __STATIC_LOOP_H
#define __STATIC_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <mutex>

class StaticLoop {
private:
  // @brief you will want to have class member to store the number of threads
  // add a public setter function for this member.
  
public:
  // @breif write setters here.
  
  // Mutex declaration
  std::mutex mtx;
  
  // Declaring vector of std::thread type
  std::vector<std::thread> threadbatch;
  
  // Declaring object variables
  int chunk_sizes[2]; int num_threads;
  
  /// @brief Run this function before running parfor. This function
  /// calculates the size that each thread's iteration ought to be.
  void static_scheduler (int n, int* chunk_sizes) {
    chunk_sizes[0] = (n - (n % num_threads)) / num_threads; // chunk for threads #1 to #(n - 1)
    chunk_sizes[1] = (n % num_threads) + (n - (n % num_threads)) / num_threads; // chunk for thread #n
  }
  
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor (size_t beg, size_t end, size_t inc,
	        std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }
  
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment, // -- size_t nbthreads,
          std::function<void(TLS&)> before,
          std::function<void(int, TLS&)> f,
          std::function<void(TLS&)> after
          ) {
    static_scheduler((end - beg), chunk_sizes);
    
    for (int n = 0; n < num_threads; ++n) {
      threadbatch.push_back (
        std::move (
          std::thread (
            [&, n]() {
              int b, e;
              if (n < num_threads - 1) {
                b = beg + (n * chunk_sizes[0]); e = ((n + 1) * chunk_sizes[0]);
              } else {
                b = beg + (n * chunk_sizes[0]); e = ((n * chunk_sizes[0]) + chunk_sizes[1]);
              }
              
              // DEBUG
              // std::cout << "~~~~~~~~~~ ~~~~~~~~~~ >> Thread #" << n << std::endl;
              // std::cout << ">>>>>>>>>> (end - beg) >>>>>>>>>> " << (end - beg) << std::endl;
              // std::cout << ">>>>>>>>>> thread info >>>>>>>>>> #" << n + 1 << "/" << num_threads << std::endl;
              // std::cout << ">>>>>>>>>> b & e info >>>>>>>>>> " << b << " --> " << e << std::endl;
              
              TLS tls;
              before(tls);
              for (size_t i=b; i<e; i+= increment) {
                f(i, tls);
              }
              mtx.lock(); after(tls); mtx.unlock();
            }
          )
        )
      );
    }
    
    for (auto &t: threadbatch) {
      if (t.joinable()) {
        t.join();
      }
    }
    
    // Original code
    // TLS tls;
    // before(tls);
    // for (size_t i=beg; i<end; i+= increment) {
    //   f(i, tls);
    // }
    // after(tls);
  }
};

#endif
