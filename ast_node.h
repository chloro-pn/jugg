#pragma once

#include <vector>

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
  size_t funcname_token_index_; //��������
  //����ֵ������
  //�����б�ÿһ��Ӧ����һ��������ÿ�����������Լ������͡�
};

class BinaryExpr : public Expression {
private:
  Expression* left_;
  Expression* right_;
};

class IdExpr : public Expression {

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