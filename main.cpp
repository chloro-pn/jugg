#include <iostream>
#include "scanner.h"
#include "parser.h"

int main() {
  auto tokens = scan("{str < tme;"
                     ";"
                     "a+b == c;"
                     "2.163;}");
  TokenPrint(tokens);
  ParseBlockStmt(tokens, 0, tokens.size() - 1);
  system("pause");
  return 0;
}