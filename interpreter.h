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
  // flag_ = 0 , ȫ�ֿռ�
  // flag_ = 1 , �����ռ�
  // flag_ = 2 , �����ռ�, ��ʱv_�洢ִ�з����ı�����
  int flag_ = 0;
};