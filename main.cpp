#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"

int main() {
  auto tokens = Scanner::instance().Scan("e://main.pn");
  Parse(tokens);
  Interpreter::instance().Exec();
  system("pause");
  return 0;
}