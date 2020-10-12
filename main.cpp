#include <iostream>
#include "scanner.h"

int main() {
  auto tokens = scan("string str;"
                     "int a;"
                     "a+b == c;"
                     "2.163;");
  TokenPrint(tokens);
  system("pause");
  return 0;
}