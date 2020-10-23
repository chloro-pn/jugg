#include "interpreter.h"
#include "type.h"
#include "ast_node.h"
#include <algorithm>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

//初始情况下在全局作用域。
Interpreter::Interpreter() :flag_(0) {

}

Variable* Interpreter::FindVariableByIdexpr(const IdExpr* expr) {
  return nullptr;
}

void Interpreter::Exec() {

}

void Interpreter::RegisterGlobalVariable(VariableDefineStmt* v) {
  auto it = std::find_if(global_var_.begin(), global_var_.end(), [v](VariableDefineStmt* each)->bool {
    return v->var_name_ == each->var_name_;
  });
  assert(it == global_var_.end());
  global_var_.push_back(v);
}