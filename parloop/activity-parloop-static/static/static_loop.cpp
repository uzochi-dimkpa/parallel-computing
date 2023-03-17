#ifndef __STATIC_LOOP_H
#define __STATIC_LOOP_H

#include <functional>
#include <vector>
#include <thread>

// #include <iostream>

class StaticLoop {
private:
  // @brief you will want to have class member to store the number of threads
  // add a public setter function for this member.

public:
  // @breif write setters here.



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
  
  // Declaring vector of thread type
  std::vector<std::thread> threadbatch;
  
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
    threadbatch.push_back (
      std::move (
        std::thread (
          [&]() {
            TLS tls;
            before(tls);    
            for (size_t i=beg; i<end; i+= increment) {
              f(i, tls);
            }
            after(tls);
          }
        )
      )
    );
    
    for (auto &t: threadbatch) {
      if (t.joinable()) {
        t.join();
      }
    }
    
    // threadbatch.push_back(std::move(thd));
    
    // std::thread thd (
    //   [&]() {
    //     TLS tls;
    //     before(tls);    
    //     for (size_t i=beg; i<end; i+= increment) {
    //       f(i, tls);
    //     }
    //     after(tls);
    //   }
    // ); thd.join();
    
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
