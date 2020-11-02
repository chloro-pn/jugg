#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "operator.h"
#include "variable.h"
#include "interpreter.h"
#include "comprehensive_type.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

// ÿ�����ʽ�ؽ�����һ�����͵�ֵ
class Expression : public AstNode {
 public:
  virtual Variable* GetVariable() = 0;
};

/*
// Ŀǰ�������������͵�֧�ֺ�ʵ��
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
    //������ִ�к���.
    std::vector<Variable*> vars;
    for (auto& each : parameters_) {
      Variable* v = each->GetVariable();
      if (v->cate_ == Variable::Category::Lvalue) {
        Variable* tmp = v->Copy(Variable::Category::Lvalue);
        vars.push_back(tmp);
      }
      else {
        //ע�⣬��ֵ��û�����ֵģ������CallFunc����Ҫ��ֵ��ÿ��Rvale��������
        v->ChangeCategory(Variable::Category::Lvalue);
        vars.push_back(v);
      }
    }
    Variable* result = Interpreter::instance().CallFunc(func_name_, vars);
    return result;
  }
};

class MethodCallExpr : public Expression {
public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override {
    Variable* obj = Interpreter::instance().FindVariableByName(var_name_);
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

class DataMemberExpr : public Expression {
public:
  std::string var_name_;
  std::string data_member_name_;
  virtual Variable* GetVariable() override {
    Variable* v = Interpreter::instance().FindVariableByName(var_name_);
    Variable* v2 = v->FindMember(data_member_name_);
    assert(v2 != nullptr);
    return v2;
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
    //��������Դ������������͵ı���
    assert(OperatorSet::instance().Get(operator_token_).FindOpFuncs(v1->type_name_.base_type_, v2->type_name_.base_type_) == true);
    Variable* result = OperatorSet::instance().Get(operator_token_).op_funcs_[{v1->type_name_.base_type_, v2->type_name_.base_type_}](v1, v2);
    if (v1->cate_ == Variable::Category::Rvalue) {
      delete v1;
    }
    if (v2->cate_ == Variable::Category::Rvalue) {
      delete v2;
    }
    return result;
  }
};

class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override {
    //���������б����󶨲�����
    return Interpreter::instance().FindVariableByName(id_name_);
  }
};

class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override {
    StringVariable* v = new StringVariable;
    v->type_name_.base_type_ = "string";
    v->val_ = str_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class IntIteralExpr : public Expression {
public:
  int64_t int_;

  Variable* GetVariable() override {
    IntVariable* v = new IntVariable;
    v->type_name_.base_type_ = "int";
    v->val_ = int_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class DoubleIteralExpr : public Expression {
public:
  double d_;
  virtual Variable* GetVariable() override {
    DoubleVariable* v = new DoubleVariable;
    v->type_name_.base_type_ = "double";
    v->val_ = d_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class BoolIteralExpr : public Expression {
public:
  bool b_;
  virtual Variable* GetVariable() override {
    BoolVariable* v = new BoolVariable;
    v->type_name_.base_type_ = "bool";
    v->val_ = b_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class ByteIteralExpr : public Expression {
public:
  uint8_t c_;
  virtual Variable* GetVariable() override {
    ByteVariable* v = new ByteVariable;
    v->type_name_.base_type_ = "byte";
    v->val_ = c_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class Statement : public AstNode {
public:
  enum class State { Continue, Break, Next, Return };
  virtual State exec() = 0;
};

class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;

  State exec() override {
    BlockContext* bc = new BlockContext;
    bc->type_ = Context::Type::Block;
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

class ForStmt : public Statement {
 public:
  Expression* init_;
  Expression* check_;
  Expression* update_;
  BlockStmt* block_;

  State exec() override {
    init_->GetVariable();
    while (true) {
      Variable* v = check_->GetVariable();
      assert(v->type_name_.base_type_ == "bool");
      bool check = static_cast<BoolVariable*>(v)->val_;
      if (v->cate_ == Variable::Category::Rvalue) {
        delete v;
      }
      if (check == true) {
        break;
      }
      State s = block_->exec();
      if (s == State::Continue) {
        continue;
      }
      else if (s == State::Break) {
        break;
      }
      else if (s == State::Return) {
        return s;
      }
      v = update_->GetVariable();
      if (v->cate_ == Variable::Category::Rvalue) {
        delete v;
      }
    }
    return State::Next;
  }
};

class IfStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* if_block_;
  BlockStmt* else_block_;

  State exec() override {
    Variable* v = check_->GetVariable();
    assert(v->type_name_.base_type_ == "bool");
    bool check = static_cast<BoolVariable*>(v)->val_;
    if (v->cate_ == Variable::Category::Rvalue) {
      delete v;
    }
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

class WhileStmt : public Statement {
 public:
  Expression* check_;
  BlockStmt* block_;

  State exec() override {
    while (true) {
      Variable* v = check_->GetVariable();
      assert(v->type_name_.base_type_ == "bool");
      bool check = static_cast<BoolVariable*>(v)->val_;
      if (v->cate_ == Variable::Category::Rvalue) {
        delete v;
      }
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

//���return_var_ == nullptr����Ϊ����䡣
class ReturnStmt : public Statement {
 public:
  Expression* return_exp_;

  State exec() override {
    Variable** return_var_ = Interpreter::instance().GetCurrentContext()->GetReturnVar();
    assert(return_var_ != nullptr);
    if (return_exp_ != nullptr) {
      Variable* tmp = return_exp_->GetVariable();
      if (tmp->cate_ == Variable::Category::Lvalue) {
        *return_var_ = tmp->Copy(Variable::Category::Rvalue);
      }
      else {
        //���صľ�����ֵ���ʲ���Ҫcopy��ֱ��ʹ�ü��ɡ�
        *return_var_ = tmp;
      }
    }
    else {
      *return_var_ = new VoidVariable;
      (*return_var_)->type_name_.base_type_ = "void";
    }
    return State::Return;
  }
};

class ExpressionStmt : public Statement {
 public:
  Expression* root_;

  State exec() override {
    Variable* v = root_->GetVariable();
    if (v->cate_ == Variable::Category::Rvalue) {
      delete v;
    }
    return State::Next;
  }
};

class VariableDefineStmt : public Statement {
 public:
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override {
    //�������ڵ�ǰ�����Ķ������
    Variable* v = CreateVariable(type_name_.base_type_);
    v->type_name_ = type_name_;
    v->id_name_ = var_name_;
    //��ʽ����ı���������ֵ��
    v->ConstructByExpression(constructors_, Variable::Category::Lvalue);
    auto& vars = Interpreter::instance().GetCurrentContext()->vars_;
    assert(vars.find(var_name_) == vars.end());
    vars[var_name_] = v;
    return State::Next;
  }
};