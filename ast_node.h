#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "operator.h"
#include "variable.h"
#include "interpreter.h"
#include "comprehensive_type.h"

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
  virtual Variable* GetVariable() override {
    std::vector<Variable*> vars;
    for (auto& each : parameters_) {
      Variable* v = each->GetVariable();
      if (v->cate_ == Variable::Category::Lvalue) {
        Variable* tmp = v->Copy(Variable::Category::Lvalue);
        vars.push_back(tmp);
      }
      else {
        v->ChangeCategory(Variable::Category::Lvalue);
        vars.push_back(v);
      }
    }
    Variable* result = Interpreter::instance().CallFunc(func_name_, vars);
    return result;
  }
};

// �������ñ��ʽ
class MethodCallExpr : public Expression {
 public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override {
    Variable* obj = Interpreter::instance().FindVariableByName(var_name_);
    assert(obj != nullptr);
    std::vector<Variable*> vars;
    for (auto& each : parameters_) {
      Variable* v = each->GetVariable();
      if (v->cate_ == Variable::Category::Lvalue) {
        Variable* tmp = v->Copy(Variable::Category::Lvalue);
        vars.push_back(tmp);
      }
      else {
        v->ChangeCategory(Variable::Category::Lvalue);
        vars.push_back(v);
      }
    }
    Variable* result = Interpreter::instance().CallMethod(obj, method_name_, vars);
    return result;
  }
};

// ���ݳ�Ա���ʱ��ʽ
class DataMemberExpr : public Expression {
 public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override {
    Variable* v = Interpreter::instance().FindVariableByName(var_name_);
    assert(v != nullptr);
    Variable* v2 = v->FindMember(data_member_name_);
    assert(v2 != nullptr);
    return v2;
  }
};

// ��Ԫ�����
class BinaryExpr : public Expression {
 public:
  Expression* left_;
  Expression* right_;
  TOKEN operator_token_;

  Variable* GetVariable() override {
    Variable* v1 = left_->GetVariable();
    Variable* v2 = right_->GetVariable();
    assert(v1 != nullptr && v2 != nullptr);
    Variable* result = nullptr;
    // ����ǻ�������
    if (v1->type_name_.IsBaseType() == true && v2->type_name_.IsBaseType() == true) {
      result = OperatorSet::instance().HandleBinary(operator_token_, v1, v2);
    }
    else {
      // ������ָ������
      assert(v1->type_name_.PtrType() && v1->type_name_ == v2->type_name_);
      result = OperatorSet::instance().HandlePtr(operator_token_, v1, v2);
    }
    Variable::HandleLife(v1);
    Variable::HandleLife(v2);
    return result;
  }
};

// ��Ԫ�����
class UnaryExpr : public Expression {
 public:
  Expression* child_;
  TOKEN operator_token_;

  Variable* GetVariable() override {
    Variable* v = child_->GetVariable();
    assert(v != nullptr);
    Variable* result = OperatorSet::instance().HandleUnary(operator_token_, v);
    Variable::HandleLife(v);
    return result;
  }
};

// id���ʽ
class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override {
    return Interpreter::instance().FindVariableByName(id_name_);
  }
};

// �ַ������������ʽ
class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override {
    StringVariable* v = new StringVariable;
    v->type_name_.base_type_ = "string";
    v->val_ = str_;
    v->cate_ = Variable::Category::Rvalue;
    v->id_name_ = "tmp";
    return v;
  }
};

// ���α��ʽ
class IntIteralExpr : public Expression {
 public:
  int64_t int_;

  Variable* GetVariable() override {
    IntVariable* v = new IntVariable;
    v->type_name_.base_type_ = "int";
    v->val_ = int_;
    v->cate_ = Variable::Category::Rvalue;
    v->id_name_ = "tmp";
    return v;
  }
};

// �����ͱ��ʽ
class DoubleIteralExpr : public Expression {
 public:
  double d_;

  virtual Variable* GetVariable() override {
    DoubleVariable* v = new DoubleVariable;
    v->type_name_.base_type_ = "double";
    v->val_ = d_;
    v->cate_ = Variable::Category::Rvalue;
    v->id_name_ = "tmp";
    return v;
  }
};

// �����ͱ��ʽ
class BoolIteralExpr : public Expression {
 public:
  bool b_;

  virtual Variable* GetVariable() override {
    BoolVariable* v = new BoolVariable;
    v->type_name_.base_type_ = "bool";
    v->val_ = b_;
    v->cate_ = Variable::Category::Rvalue;
    v->id_name_ = "tmp";
    return v;
  }
};

// �ֽ��ͱ��ʽ
class ByteIteralExpr : public Expression {
 public:
  uint8_t c_;

  virtual Variable* GetVariable() override {
    ByteVariable* v = new ByteVariable;
    v->type_name_.base_type_ = "byte";
    v->val_ = c_;
    v->cate_ = Variable::Category::Rvalue;
    v->id_name_ = "tmp";
    return v;
  }
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

  State exec() override {
    BlockContext* bc = new BlockContext(Context::Type::Block);
    Interpreter::instance().Enter(bc);
    for (auto& each : block_) {
      State s = each->exec();
      if (s != State::Next) {
        Interpreter::instance().Leave();
        return s;
      }
    }
    Interpreter::instance().Leave();
    return State::Next;
  }
};

// ����/������䣬��ʹ�ÿ�����ԭ���Ǻ���/���������ĵĽ����ɵ����߿��ƣ����
// exec�в����������ĵļ�¼��
class FMBlockStmt : public BlockStmt {
 public:
  State exec() override {
    for (auto& each : this->block_) {
      State s = each->exec();
      if (s != State::Next) {
        return s;
      }
    }
    return State::Next;
  }
};

// for���
class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;

  State exec() override {
    Variable* v = init_->GetVariable();
    while (true) {
      Variable* v = check_->GetVariable();
      assert(v->type_name_.IsBaseType("bool") == true);
      bool check = VariableCast<BoolVariable>(v)->val_;
      Variable::HandleLife(v);
      if (check == false) {
        break;
      }
      State s = block_->exec();
      if (s == State::Break) {
        break;
      }
      else if (s == State::Return) {
        return s;
      }
      v = update_->GetVariable();
      Variable::HandleLife(v);
    }
    return State::Next;
  }
};

// if���
class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;

  State exec() override {
    Variable* v = check_->GetVariable();
    assert(v->type_name_.IsBaseType("bool"));
    bool check = VariableCast<BoolVariable>(v)->val_;
    Variable::HandleLife(v);
    if (check == true) {
      State s = if_block_->exec();
      if (s != State::Next) {
        return s;
      }
    }
    else if (else_block_ != nullptr) {
      State s = else_block_->exec();
      if (s != State::Next) {
        return s;
      }
    }
    return State::Next;
  }
};

// while���
class WhileStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* block_;

  State exec() override {
    while (true) {
      Variable* v = check_->GetVariable();
      assert(v->type_name_.IsBaseType("bool"));
      bool check = VariableCast<BoolVariable>(v)->val_;
      Variable::HandleLife(v);
      if (check == false) {
        break;
      }
      State s = block_->exec();
      if (s == State::Break) {
        break;
      }
      else if (s == State::Continue) {
        continue;
      }
      else if (s == State::Return) {
        return s;
      }
    }
    return State::Next;
  }
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

  State exec() override {
    Variable** return_var_ = Interpreter::instance().GetCurrentContext()->GetReturnVar();
    assert(return_var_ != nullptr);
    //���return_var_�ǿ�ָ�룬��return����ǿ���䡣
    if (return_exp_ != nullptr) {
      Variable* tmp = return_exp_->GetVariable();
      assert(tmp != nullptr);
      if (tmp->IsLValue()) {
        *return_var_ = tmp->Copy(Variable::Category::Rvalue);
      }
      else {
        *return_var_ = tmp;
      }
    }
    else {
      *return_var_ = new VoidVariable;
      (*return_var_)->type_name_.base_type_ = "void";
      (*return_var_)->id_name_ = "tmp";
      (*return_var_)->cate_ = Variable::Category::Rvalue;
    }
    return State::Return;
  }
};

// ���ʽ���
class ExpressionStmt : public Statement {
 public:
  Expression* root_;

  State exec() override {
    Variable* v = root_->GetVariable();
    assert(v != nullptr);
    Variable::HandleLife(v);
    return State::Next;
  }
};

// �����������
class VariableDefineStmt : public Statement {
 public:
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override {
    //�������ڵ�ǰ�����Ķ������
    Variable* v = CreateVariable(type_name_);
    v->type_name_ = type_name_;
    v->id_name_ = var_name_;
    //��ʽ����ı���������ֵ��
    v->ConstructByExpression(constructors_, Variable::Category::Lvalue);
    Interpreter::instance().GetCurrentContext()->RegisterVariable(v);
    return State::Next;
  }
};