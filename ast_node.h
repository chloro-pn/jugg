#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "operator.h"
#include "variable.h"
#include "interpreter.h"
#include "comprehensive_type.h"

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

// 方法调用表达式
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

// 数据成员访问表达式
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

// 二元运算符
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
    // 如果是基本类型
    if (v1->type_name_.IsBaseType() == true && v2->type_name_.IsBaseType() == true) {
      result = OperatorSet::instance().HandleBinary(operator_token_, v1, v2);
    }
    else {
      // 否则是指针类型
      assert(v1->type_name_.PtrType() && v1->type_name_ == v2->type_name_);
      result = OperatorSet::instance().HandlePtr(operator_token_, v1, v2);
    }
    Variable::HandleLife(v1);
    Variable::HandleLife(v2);
    return result;
  }
};

// 单元运算符
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

// id表达式
class IdExpr : public Expression {
 public:
  std::string id_name_;

  Variable* GetVariable() override {
    return Interpreter::instance().FindVariableByName(id_name_);
  }
};

// 字符串字面量表达式
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

// 整形表达式
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

// 浮点型表达式
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

// 布尔型表达式
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

// 字节型表达式
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

// 函数/方法语句，不使用块语句的原因是函数/方法上下文的进入由调用者控制，因此
// exec中不进行上下文的记录。
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

// for语句
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

// if语句
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

// while语句
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
    //如果return_var_是空指针，则return语句是空语句。
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

// 表达式语句
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

// 变量定义语句
class VariableDefineStmt : public Statement {
 public:
  ComprehensiveType type_name_;
  std::string var_name_;
  std::vector<Expression*> constructors_;

  State exec() override {
    //解释器在当前上下文定义变量
    Variable* v = CreateVariable(type_name_);
    v->type_name_ = type_name_;
    v->id_name_ = var_name_;
    //显式定义的变量都是左值。
    v->ConstructByExpression(constructors_, Variable::Category::Lvalue);
    Interpreter::instance().GetCurrentContext()->RegisterVariable(v);
    return State::Next;
  }
};