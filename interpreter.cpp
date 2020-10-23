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