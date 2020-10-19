#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "scanner.h"
#include "variable.h"
#include "operator.h"
#include "func.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

// ÿ�����ʽ�ؽ�����һ�����͵�ֵ
class Expression : public AstNode {
 public:
  std::string type_name_;

  virtual std::string TypeDeduction() = 0;
};

// Ŀǰ�������������͵�֧�ֺ�ʵ��
class IndexExpr : public Expression {
 private:
  size_t token_index_;
  Expression* index_expr_;
};

class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;

  virtual std::string TypeDeduction() override {
    assert(FuncSet::instance().Find(func_name_) == true);
    return FuncSet::instance().Get(func_name_).return_type_;
  }
};

class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;

  virtual std::string TypeDeduction() override {
    std::string left_type = left_->TypeDeduction();
    std::string right_type = right_->TypeDeduction();
    assert(OperatorSet::instance().find(operator_token_) == true);
    return OperatorSet::instance().get(operator_token_).GetReturnType(left_type, right_type);
  }
};

//ÿ��Id���ʽ��Ҫ���󶨵�ĳ���������ϡ�
//������Ҫִ�о�̬�󶨣�Ȼ��ִ�б��ʽ�����Ƶ����顣
class IdExpr : public Expression {
 public:
  std::string id_name_;
  size_t scope_index_;
  Variable* var_;

  virtual std::string TypeDeduction() override {
    return var_->type_name_;
 }
};

class IteralExpr : public Expression {
 public:
  Token token_;
  virtual std::string TypeDeduction() override {
    if (token_.type == Token::TYPE::STRING) {
      return "string";
    }
    else if (token_.type == Token::TYPE::INT) {
      return "int";
    }
    else if (token_.type == Token::TYPE::DOUBLE) {
      return "double";
    }
    else if (token_.type == Token::TYPE::BOOL) {
      return "bool";
    }
    else if (token_.type == Token::TYPE::CHAR) {
      return "char";
    }
    else {
      std::cerr << "error iteral type." << std::endl;
      exit(EXIT_FAILURE);
    }
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

//���return_var_ == nullptr����Ϊ����䡣
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