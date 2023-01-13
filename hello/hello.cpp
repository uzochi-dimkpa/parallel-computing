#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main () {
  //...
  char hName[256];
  int out = gethostname(hName, sizeof(hName));

  std::cout << "Hostname: " << hName << "\n";

  return 0;
}