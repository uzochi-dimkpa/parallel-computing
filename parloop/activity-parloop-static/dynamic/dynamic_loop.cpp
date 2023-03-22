#ifndef __DYNAMIC_LOOP_H
#define __DYNAMIC_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <mutex>

#include <iostream>

class DynamicLoop {
private:
  // @brief you will want to have class members to store the number of threads
  // and the desired granularity. Add a public setter functions for these
  // class members.
  
public:
  // @breif write setters here.
  
  // Mutex declaration
  std::mutex mtx1, mtx2;
  
  // Declaring vector of std::thread type
  std::vector<std::thread> threadbatch;
  
  // Declaring object variables
  int num_threads, granularity, begtoend;
  
  /// @brief Run this function before running parfor. This function
  /// calculates the size that each thread's iteration ought to be.
  void dynamic_scheduler (int n) {
    if (granularity <= 0) {
      granularity = 1;
    } else if (granularity > n) {
      granularity = n;
    }
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
  void parfor (size_t beg, size_t end, size_t increment,
          std::function<void(TLS&)> before,
          std::function<void(int, TLS&)> f,
          std::function<void(TLS&)> after
          ) {
    begtoend = (end - beg);
    dynamic_scheduler(begtoend);
    // // DEBUG
    // std::cout << ">>>>>>>>>> # threads: " << num_threads << std::endl;
    // std::cout << ">>>>>>>>>> granularity: " << granularity << std::endl;
    // std::cout << ">>>>>>>>>> remainder: " << chunk_sizes[1] << std::endl;
    // std::cout << ">>>>>>>>>> # grains: " << num_grains << std::endl;
    
    for (int nt = 0; nt < num_threads; ++nt) {
      threadbatch.push_back (
        std::move (
          std::thread (
            [&, nt]() {
              int b, e;
              while (begtoend > 0) {
                mtx2.lock();
                b = (begtoend - granularity); e = begtoend;
                if ((b < 0) && ((begtoend % granularity) != 0)) {
                  b = 0; e = begtoend % granularity;
                }; begtoend -= granularity; mtx2.unlock();
                
                // // DEBUG
                // std::cout << "&&&&&&&&&&\n" << std::endl;
                // std::cout << "~~~~~~~~~~ ~~~~~~~~~~ >> Thread #" << nt + 1 << std::endl;
                // std::cout << ">>>>>>>>>> (end - beg) >>>>>>>>>> " << (end - beg) << std::endl;
                // std::cout << ">>>>>>>>>> thread info >>>>>>>>>> #" << nt + 1 << "/" << num_threads << std::endl;
                // std::cout << ">>>>>>>>>> b & e info >>>>>>>>>> " << b << " --> " << e << std::endl;
                // std::cout << "\n&&&&&&&&&&" << std::endl;
                
                TLS tls;
                before(tls);
                for (size_t i=b; i<e; i+= increment) {
                  f(i, tls);
                }
                
                mtx1.lock(); after(tls); mtx1.unlock();
              }
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
