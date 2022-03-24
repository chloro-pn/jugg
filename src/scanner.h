#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <any>
#include <cassert>

struct Token {
  TOKEN token;
  enum class TYPE {STRING, INT, DOUBLE, BYTE, BOOL, NONE};
  TYPE type;
  std::any attr;

  template<typename T>
  T get() const {
    return std::any_cast<T>(attr);
  }

  Token() = default;

  Token(const Token& other) = default;
};

void TokenPrint(const std::vector<Token>& tokens);

class Scanner {
 public:
  static Scanner& instance();
  const std::vector<Token>& Scan(const std::string& filename);

 private:
  Scanner();
  std::vector<Token> tokens_;
};