#include <iostream>
#include "scanner.h"
#include "parser.h"

int main() {
  auto tokens = scan("while(i > 3) { i = i + 1;}");
  TokenPrint(tokens);
  size_t next = 0;
  ParseStatement(tokens, 0, next);
  system("pause");
  return 0;
}