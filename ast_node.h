#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "scanner.h"
#include "operator.h"
#include "func.h"
#include "variable.h"
#include "interpreter.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

// 每个表达式必将返回一个类型的值
class Expression : public AstNode {
 public:
  std::string type_name_;
  virtual Variable* GetVariable() = 0;
};

/*
// 目前不考虑数组类型的支持和实现
class IndexExpr : public Expression {
 private:
  size_t token_index_;
  Expression* index_expr_;
};
*/

class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override {
    //解释器执行函数.
    return nullptr;
  }
};

class MethodCallExpr : public Expression {
public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class DataMemberExpr : public Expression {
public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;

  Variable* GetVariable() override {
    Variable* v1 = left_->GetVariable();
    Variable* v2 = right_->GetVariable();
    //运算符可以处理这两个类型的变量
    assert(OperatorSet::instance().Get(operator_token_).FindOpFuncs(v1->type_name_, v2->type_name_) == true);
    Variable* result = OperatorSet::instance().Get(operator_token_).op_funcs_[{v1->type_name_, v2->type_name_}](v1, v2);
    return result;
  }
};

class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override {
    //解释器进行变量绑定并返回
    return Interpreter::instance().FindVariableByIdexpr(this);
  }
};

class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override {
    StringVariable* v = new StringVariable;
    v->type_name_ = "string";
    v->val_ = str_;
    return v;
  }
};

class IntIteralExpr : public Expression {
public:
  int64_t int_;

  Variable* GetVariable() override {
    IntVariable* v = new IntVariable;
    v->type_name_ = "int";
    v->val_ = int_;
    return v;
  }
};

class DoubleIteralExpr : public Expression {
public:
  double d_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class BoolIteralExpr : public Expression {
public:
  bool b_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class CharIteralExpr : public Expression {
public:
  char c_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class Statement : public AstNode {

};

class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;
};

class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;
};

class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;
};

class WhileStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* block_;
};

class BreakStmt : public Statement {

};

class ContinueStmt : public Statement {

};

//如果return_var_ == nullptr，则为空语句。
class ReturnStmt : public Statement {
 public:
  Expression* return_var_;
};

class ExpressionStmt : public Statement {
 public:
  Expression* root_;
};

class VariableDefineStmt : public Statement {
 public:
  std::string type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;
  size_t scope_index_;
};