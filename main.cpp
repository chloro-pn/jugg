#include <iostream>
#include "scanner.h"
#include "parser.h"

int main() {
  Scanner scanner("e://main.pn");
  Parse(scanner.tokens());
  TokenPrint(scanner.tokens());
  system("pause");
  return 0;
}