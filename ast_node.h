#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "comprehensive_type.h"
class Variable;

// 类AstNode是抽象语法树节点的基类。
class AstNode {
 public:
  virtual ~AstNode() = default;
};

// 表达式类
class Expression : public AstNode {
 public:
  virtual Variable* GetVariable() = 0;
};

// 函数调用表达式
class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// 方法调用表达式
class MethodCallExpr : public Expression {
 public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// 数据成员访问表达式
class DataMemberExpr : public Expression {
 public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override;
};

// 二元运算符
class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;
  Variable* GetVariable() override;
};

// 单元运算符
class UnaryExpr : public Expression {
 public:
  Expression* child_;
  TOKEN operator_token_;

  Variable* GetVariable() override;
};

// id表达式
class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override;
};

// 字符串字面量表达式
class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override;
};

// 整形表达式
class IntIteralExpr : public Expression {
 public:
  int64_t int_;

  Variable* GetVariable() override;
};

// 浮点型表达式
class DoubleIteralExpr : public Expression {
 public:
  double d_;

  virtual Variable* GetVariable() override;
};

// 布尔型表达式
class BoolIteralExpr : public Expression {
 public:
  bool b_;

  virtual Variable* GetVariable() override;
};

// 字节型表达式
class ByteIteralExpr : public Expression {
 public:
  uint8_t c_;

  virtual Variable* GetVariable() override;
};

// 语句, 每个语句都可以被执行(exec)，并具有返回值。
// 这个返回值对语言使用者不可见，主要是为了实现break, return和continue等
// 规则。
class Statement : public AstNode {
public:
  enum class State { Continue, Break, Next, Return };
  virtual State exec() = 0;
};

// 块语句
class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;

  State exec() override;
};

// 函数/方法语句，不使用块语句的原因是函数/方法上下文的进入由调用者控制，因此
// exec中不进行上下文的记录。
class FMBlockStmt : public BlockStmt {
 public:
  State exec() override;
};

// for语句
class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;

  State exec() override;
};

// if语句
class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;

  State exec() override;
};

// while语句
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

// 表达式语句
class ExpressionStmt : public Statement {
 public:
  Expression* root_;

  State exec() override;
};

// 变量定义语句
class VariableDefineStmt : public Statement {
 public:
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override;
};