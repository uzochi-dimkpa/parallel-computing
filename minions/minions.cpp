#include <iostream>
#include <thread>
#include <vector>


int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  //Your code goes here
  std::vector<std::thread> threadbatch;

  for (int i = 0; i < atoi(argv[1]); i++) {
    std::thread minion (
      [i]() {
        std::cout << "Hello! I am minion " << i + 1 << std::endl;
      }
    ); // -- minion.join();
    
    threadbatch.push_back(std::move(minion));
  }

  for (auto &t: threadbatch) {
    if (t.joinable()) {
      t.join();
    }
  }

  std::thread overlord (
    []() {
      std::cout << "Hello minions! I am the Overlord!" << std::endl;
    }
  ); overlord.join();

  return 0;
}
