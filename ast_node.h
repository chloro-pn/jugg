#pragma once

#include <vector>
#include "scanner.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

class Expression : public AstNode {

};

class IndexExpr : public Expression {
 private:
  size_t token_index_;
  Expression* index_expr_;
};

class FuncCallExpr : public Expression {
 private:
  size_t funcname_token_index_; //函数名称
  //返回值的类型
  //参数列表，每一项应该是一个变量，每个变量具有自己的类型。
};

class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;
};

class IdExpr : public Expression {
public:
  size_t token_index_;
};

class IteralExpr : public Expression {

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

class ReturnStmt : public Statement {

};

class ExpressionStmt : public Statement {
 public:
  Expression* root_;
};