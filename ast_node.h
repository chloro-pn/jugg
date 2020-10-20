#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "scanner.h"
#include "operator.h"
#include "func.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

// 每个表达式必将返回一个类型的值
class Expression : public AstNode {
 public:
  std::string type_name_;
};

// 目前不考虑数组类型的支持和实现
class IndexExpr : public Expression {
 private:
  size_t token_index_;
  Expression* index_expr_;
};

class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;
};

class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;
};

class IdExpr : public Expression {
 public:
  std::string id_name_;
  size_t scope_index_;
};

class IteralExpr : public Expression {
 public:
  Token token_;
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
};