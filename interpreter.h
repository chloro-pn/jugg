#pragma once
#include "variable.h"
#include "func.h"
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
class IdExpr;
class VariableDefineStmt;

class Interpreter {
public:
  static Interpreter& instance();
  Variable* FindVariableByIdexpr(const IdExpr*);
  int Flag() const {
    return flag_;
  }
  void RegisterGlobalVariable(VariableDefineStmt* v);
  void Exec();
  void CallFunc(const std::string& func_name);

private:
  Interpreter();
  std::stack<FuncContext> func_;
  std::stack<std::pair<Variable*, std::string>> method_;
  std::vector<VariableDefineStmt*> global_var_;
  std::unordered_map<std::string, Variable*> gv_;
  // flag_ = 0 , 全局空间
  // flag_ = 1 , 函数空间, 此时func_顶部为所在函数上下文
  // flag_ = 2 , 方法空间, 此时method_顶部为方法所在上下文
  std::stack<int> flag_;
};