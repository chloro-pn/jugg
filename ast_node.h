#pragma once

#include <vector>
#include <string>
#include "scanner.h"
#include "variable.h"

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
  Variable* var_;
  std::vector<Expression*> constructors_;
};