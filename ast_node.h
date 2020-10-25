#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "scanner.h"
#include "operator.h"
#include "func.h"
#include "variable.h"
#include "interpreter.h"

class AstNode {
 public:
  virtual ~AstNode() = default;
};

// 每个表达式必将返回一个类型的值
class Expression : public AstNode {
 public:
  virtual Variable* GetVariable() = 0;
};

/*
// 目前不考虑数组类型的支持和实现
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
    //解释器执行函数.
    std::vector<Variable*> vars;
    for (auto& each : parameters_) {
      vars.push_back(each->GetVariable());
    }
    Variable* result = Interpreter::instance().CallFunc(func_name_, vars);
    for (auto& each : vars) {
      if (each->cate_ == Variable::Category::Rvalue) {
        delete each;
      }
    }
    return result;
  }
};

class MethodCallExpr : public Expression {
public:
  std::string var_name_;
  std::string method_name_;
  std::vector<Expression*> parameters_;
  virtual Variable* GetVariable() override {
    return nullptr;
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
    //运算符可以处理这两个类型的变量
    assert(OperatorSet::instance().Get(operator_token_).FindOpFuncs(v1->type_name_, v2->type_name_) == true);
    Variable* result = OperatorSet::instance().Get(operator_token_).op_funcs_[{v1->type_name_, v2->type_name_}](v1, v2);
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
    //解释器进行变量绑定并返回
    return Interpreter::instance().FindVariableByName(id_name_);
  }
};

class StringIteralExpr : public Expression {
 public:
  std::string str_;

  Variable* GetVariable() override {
    StringVariable* v = new StringVariable;
    v->type_name_ = "string";
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
    v->type_name_ = "int";
    v->val_ = int_;
    v->cate_ = Variable::Category::Rvalue;
    return v;
  }
};

class DoubleIteralExpr : public Expression {
public:
  double d_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class BoolIteralExpr : public Expression {
public:
  bool b_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class CharIteralExpr : public Expression {
public:
  char c_;
  virtual Variable* GetVariable() override {
    return nullptr;
  }
};

class Statement : public AstNode {
public:
  enum class State { Continue, Break, Next, Return };
  virtual State exec() = 0;
  // 这个函数为了向return语句注册函数返回值
  virtual void RegisterReturnVar(Variable*& rv) {
    ;
  }
};

class BlockStmt : public Statement {
 public:
  std::vector<Statement*> block_;

  State exec() override {
    for (auto& each : block_) {
      State s = each->exec();
      if (s != State::Next) {
        return s;
      }
    }
    return State::Next;
  }

  void RegisterReturnVar(Variable*& rv) override {
    for (auto& each : block_) {
      each->RegisterReturnVar(rv);
    }
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
      assert(v->type_name_ == "bool");
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
    assert(v->type_name_ == "bool");
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
      assert(v->type_name_ == "bool");
      bool check = static_cast<BoolVariable*>(v)->val_;
      if (v->cate_ == Variable::Category::Rvalue) {
        delete v;
      }
      if (check == true) {
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

//如果return_var_ == nullptr，则为空语句。
class ReturnStmt : public Statement {
 public:
  Expression* return_exp_;
  Variable** return_var_;

  void RegisterReturnVar(Variable*& rv) override {
    return_var_ = &rv;
  }

  State exec() override {
    if (return_exp_ != nullptr) {
      Variable* tmp = return_exp_->GetVariable();
      if (tmp->cate_ == Variable::Category::Lvalue) {
        *return_var_ = tmp->Copy();
        (*return_var_)->cate_ = Variable::Category::Rvalue;
      }
      else {
        //返回的就是右值，故不需要copy，直接使用即可。
        *return_var_ = tmp;
      }
    }
    else {
      *return_var_ = nullptr;
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
  std::string type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override {
    //解释器在当前上下文定义变量
    Variable* v = CreateVariable(type_name_);
    v->type_name_ = type_name_;
    v->id_name_ = var_name_;
    //显式定义的变量都是左值。
    v->cate_ = Variable::Category::Lvalue;
    v->ConstructByExpression(constructors_);
    auto& vars = Interpreter::instance().GetCurrentContext()->vars_;
    assert(vars.find(var_name_) == vars.end());
    vars[var_name_] = v;
    return State::Next;
  }
};