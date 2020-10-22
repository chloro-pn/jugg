#pragma once
#include "variable.h"
#include <string>
#include <stack>
class IdExpr;

class Interpreter {
public:
  static Interpreter& instance();
  Variable* FindVariableByIdexpr(const IdExpr*);
  void Exec();

private:
  Interpreter();
  std::stack<std::string> func_;
  std::stack<std::pair<std::string, std::string>> type_method_;
  std::stack<Variable*> v_;
  // flag_ = 0 , 全局空间
  // flag_ = 1 , 函数空间
  // flag_ = 2 , 方法空间, 此时v_存储执行方法的变量。
  int flag_ = 0;
};