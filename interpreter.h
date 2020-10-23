#pragma once
#include "variable.h"
#include <string>
#include <stack>
class IdExpr;

class Interpreter {
public:
  static Interpreter& instance();
  Variable* FindVariableByIdexpr(const IdExpr*);
  int Flag() const {
    return flag_;
  }
  void Exec();

private:
  Interpreter();
  std::stack<std::string> func_;
  std::stack<std::pair<Variable*, std::string>> method_;
  // flag_ = 0 , 全局空间
  // flag_ = 1 , 函数空间, 此时func_顶部为所在函数上下文
  // flag_ = 2 , 方法空间, 此时method_顶部为方法所在上下文
  int flag_ = 0;
};