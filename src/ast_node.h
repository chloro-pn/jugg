#pragma once
#include <string>
#include <vector>
#include "token.h"
#include "comprehensive_type.h"
class Variable;

// ��AstNode�ǳ����﷨���ڵ�Ļ��ࡣ
class AstNode {
 public:
  virtual ~AstNode() = default;
};

// ���ʽ��
class Expression : public AstNode {
 public:
  virtual Variable* GetVariable() = 0;
};

// �������ñ��ʽ
class FuncCallExpr : public Expression {
 public:
  std::string func_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// �������ñ��ʽ
class MethodCallExpr : public Expression {
 public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override;
};

// ���ݳ�Ա���ʱ��ʽ
class DataMemberExpr : public Expression {
 public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override;
};

// ��Ԫ�����
class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;
  Variable* GetVariable() override;
};

// ��Ԫ�����
class UnaryExpr : public Expression {
 public:
  Expression* child_;
  TOKEN operator_token_;

  Variable* GetVariable() override;
};

// id���ʽ
class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override;
};

// �ַ������������ʽ
class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override;
};

// ���α��ʽ
class IntIteralExpr : public Expression {
 public:
  int64_t int_;

  Variable* GetVariable() override;
};

// �����ͱ��ʽ
class DoubleIteralExpr : public Expression {
 public:
  double d_;

  virtual Variable* GetVariable() override;
};

// �����ͱ��ʽ
class BoolIteralExpr : public Expression {
 public:
  bool b_;

  virtual Variable* GetVariable() override;
};

// �ֽ��ͱ��ʽ
class ByteIteralExpr : public Expression {
 public:
  uint8_t c_;

  virtual Variable* GetVariable() override;
};

// ���, ÿ����䶼���Ա�ִ��(exec)�������з���ֵ��
// �������ֵ������ʹ���߲��ɼ�����Ҫ��Ϊ��ʵ��break, return��continue��
// ����
class Statement : public AstNode {
public:
  enum class State { Continue, Break, Next, Return };
  virtual State exec() = 0;
};

// �����
class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;

  State exec() override;
};

// ����/������䣬��ʹ�ÿ�����ԭ���Ǻ���/���������ĵĽ����ɵ����߿��ƣ����
// exec�в����������ĵļ�¼��
class FMBlockStmt : public BlockStmt {
 public:
  State exec() override;
};

// for���
class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;

  State exec() override;
};

// if���
class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;

  State exec() override;
};

// while���
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

// ���ʽ���
class ExpressionStmt : public Statement {
 public:
  Expression* root_;

  State exec() override;
};

// �����������
class VariableDefineStmt : public Statement {
 public:
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override;
};