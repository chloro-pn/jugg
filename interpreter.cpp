#include "interpreter.h"
#include "type.h"
#include "ast_node.h"
#include <algorithm>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

//��ʼ�������ȫ��������
Interpreter::Interpreter() :flag_(0) {

}

Variable* Interpreter::FindVariableByIdexpr(const IdExpr* expr) {
  return nullptr;
}

void Interpreter::Exec() {

}