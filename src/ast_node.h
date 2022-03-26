#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "comprehensive_type.h"
class Variable;
class AstNode {
 public:
  virtual ~AstNode() = default;
};

// 所有表达式对象的基类
class Expression : public AstNode {
 public:
  virtual Variable* GetVariable() = 0;
};

// new:type_name(expr, ..)
class NewVarExpr : public Expression {
 public:
  ComprehensiveType type_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// fn func_name(param_list)
class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// obj.method_name(param_list)
class MethodCallExpr : public Expression {
 public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// boj.data_member
class DataMemberExpr : public Expression {
 public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override;
};

// 二元运算
class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;
  Variable* GetVariable() override;
};

// 单元运算
class UnaryExpr : public Expression {
 public:
  Expression* child_;
  TOKEN operator_token_;

  Variable* GetVariable() override;
};

// 标识符表达式
class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override;
};

// 索引表达式
class IndexExpr : public Expression {
 public:
  std::string id_name_;
  int64_t index_;

  Variable* GetVariable() override;
};

// 字符串字面量表达式
class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override;
};

// 整形字面量表达式
class IntIteralExpr : public Expression {
 public:
  int64_t int_;

  Variable* GetVariable() override;
};

// 浮点型字面量表达式
class DoubleIteralExpr : public Expression {
 public:
  double d_;

  virtual Variable* GetVariable() override;
};

// 布尔型字面量表达式
class BoolIteralExpr : public Expression {
 public:
  bool b_;

  virtual Variable* GetVariable() override;
};

// byte型字面量表达式
class ByteIteralExpr : public Expression {
 public:
  uint8_t c_;

  virtual Variable* GetVariable() override;
};

// 所有语句的基类，包括块语句、流程控制语句、函数/方法调用语句、变量定义语句
class Statement : public AstNode {
public:
  enum class State { Continue, Break, Next, Return };
  virtual State exec() = 0;
};

class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;

  State exec() override;
};

class FMBlockStmt : public BlockStmt {
 public:
  State exec() override;
};

class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;

  State exec() override;
};

class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;

  State exec() override;
};

class WhileStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* block_;

  State exec() override;
};

class BreakStmt : public Statement {
public:
  State exec() override {
    return State::Break;
  }
};

class ContinueStmt : public Statement {
public:
  State exec() override {
    return State::Continue;
  }
};

class ReturnStmt : public Statement {
 public:
  Expression* return_exp_;

  State exec() override;
};

class ExpressionStmt : public Statement {
 public:
  Expression* root_;

  State exec() override;
};

class VariableDefineStmt : public Statement {
 public:
  enum class TYPE {DIRECT, COPY};
  TYPE type_;
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override;
};