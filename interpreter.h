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
  // flag_ = 0 , ȫ�ֿռ�
  // flag_ = 1 , �����ռ�, ��ʱfunc_����Ϊ���ں���������
  // flag_ = 2 , �����ռ�, ��ʱmethod_����Ϊ��������������
  int flag_ = 0;
};