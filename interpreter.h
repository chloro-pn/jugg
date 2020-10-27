#pragma once
#include "variable.h"
#include "context.h"
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_map>
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
  bool FindInnerFunc(const std::string& func_name) {
    return inner_func_.find(func_name) != inner_func_.end();
  }
  Variable* CallFunc(const std::string& func_name, const std::vector<Variable*>& variables);
  Variable* CallMethod(Variable* obj, const std::string& method_name, const std::vector<Variable*>& variables);

private:
  Interpreter();
  std::stack<Context*> context_;
  std::vector<VariableDefineStmt*> global_var_;
  std::unordered_map<std::string, std::function<void(FuncContext*)>> inner_func_;
};