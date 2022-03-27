#include <iostream>
#include <cstdlib>
#include "scanner.h"

int main(int argv, char* argc[]) {
  if(argv != 2) {
    std::cerr << "please input source file." << std::endl;
    exit(EXIT_FAILURE);
  }
  auto tokens = Scanner::instance().Scan(argc[1]);
  TokenPrint(tokens);
  return 0;
}