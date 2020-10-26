#pragma once
#include "variable.h"
#include "context.h"
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
class VariableDefineStmt;

class Interpreter {
public:
  static Interpreter& instance();
  Variable* FindVariableByName(const std::string&);
  void Enter(BlockContext* c) {
    context_.push(c);
  }
  void Leave() {
    Context* c = context_.top();
    context_.pop();
    for (auto& each : c->vars_) {
      delete each.second;
    }
    delete c;
  }
  Context* GetCurrentContext() {
    return context_.top();
  }
  void RegisterGlobalVariable(VariableDefineStmt* v);
  void Exec();
  Variable* CallFunc(const std::string& func_name, const std::vector<Variable*>& variables);
  Variable* CallMethod(Variable* obj, const std::string& method_name, const std::vector<Variable*>& variables);

private:
  Interpreter();
  std::stack<Context*> context_;
  std::vector<VariableDefineStmt*> global_var_;
};