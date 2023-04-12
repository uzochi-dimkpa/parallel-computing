#include <mpi.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char*argv[]) {
  char hName[256];
  int out = gethostname(hName, sizeof(hName));

  std::cout << "Hostname: " << hName << "\n";
}
