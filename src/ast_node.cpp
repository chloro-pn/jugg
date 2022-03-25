#include <cstdlib>
#include <vector>
#include <string>

#include "ast_node.h"
#include "operator.h"
#include "variable.h"
#include "interpreter.h"

// new表达式创建的都是右值对象
Variable* NewVarExpr::GetVariable() {
  Variable* result = CreateVariable(type_name_);
  result->type_name_.base_type_ = type_name_.base_type_;
  result->id_name_ = "tmp";
  result->ConstructByExpression(parameters_, Variable::Category::Rvalue);
  return result;
}

Variable* FuncCallExpr::GetVariable() {
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

Variable* MethodCallExpr::GetVariable() {
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

Variable* DataMemberExpr::GetVariable() {
  Variable* v = Interpreter::instance().FindVariableByName(var_name_);
  assert(v != nullptr);
  Variable* v2 = v->FindMember(data_member_name_);
  assert(v2 != nullptr);
  return v2;
}

Variable* BinaryExpr::GetVariable() {
  Variable* v1 = left_->GetVariable();
  Variable* v2 = right_->GetVariable();
  assert(v1 != nullptr && v2 != nullptr);
  Variable* result = nullptr;
  result = OperatorSet::instance().HandleBinary(operator_token_, v1, v2);
  Variable::HandleLife(v1);
  Variable::HandleLife(v2);
  return result;
}

Variable* UnaryExpr::GetVariable() {
  Variable* v = child_->GetVariable();
  assert(v != nullptr);
  Variable* result = OperatorSet::instance().HandleUnary(operator_token_, v);
  Variable::HandleLife(v);
  return result;
}

Variable* IdExpr::GetVariable() {
  return Interpreter::instance().FindVariableByName(id_name_);
}

Variable* IndexExpr::GetVariable() {
  Variable* v = Interpreter::instance().FindVariableByName(id_name_);
  // todo 增强类型检测能力，这里直接转换了
  auto tmp = VariableCast<ArrayVariable>(v);
  return tmp->At(index_);
}

Variable* StringIteralExpr::GetVariable() {
  StringVariable* v = new StringVariable;
  v->type_name_.base_type_ = "string";
  v->val_ = str_;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  return v;
}

Variable* IntIteralExpr::GetVariable() {
  IntVariable* v = new IntVariable;
  v->type_name_.base_type_ = "int";
  v->val_ = int_;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  return v;
}

Variable* DoubleIteralExpr::GetVariable() {
  DoubleVariable* v = new DoubleVariable;
  v->type_name_.base_type_ = "double";
  v->val_ = d_;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  return v;
}

Variable* BoolIteralExpr::GetVariable() {
  BoolVariable* v = new BoolVariable;
  v->type_name_.base_type_ = "bool";
  v->val_ = b_;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  return v;
}

Variable* ByteIteralExpr::GetVariable() {
  ByteVariable* v = new ByteVariable;
  v->type_name_.base_type_ = "byte";
  v->val_ = c_;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  return v;
}

Statement::State BlockStmt::exec() {
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

Statement::State FMBlockStmt::exec() {
  for (auto& each : this->block_) {
    State s = each->exec();
    if (s != State::Next) {
      return s;
    }
  }
  return State::Next;
}

Statement::State ForStmt::exec() {
  Variable* v = init_->GetVariable();
  Variable::HandleLife(v);
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

Statement::State IfStmt::exec() {
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

Statement::State WhileStmt::exec() {
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

Statement::State ReturnStmt::exec() {
  // 获取当前上下文的存储return_var的位置
  Variable** return_var_ = Interpreter::instance().GetReturnVar();
  assert(return_var_ != nullptr);
  // 如果return表达式不为空
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
    // return 表达式为空，填充Void类型的对象
    *return_var_ = new VoidVariable;
    (*return_var_)->type_name_.base_type_ = "void";
    (*return_var_)->id_name_ = "tmp";
    (*return_var_)->cate_ = Variable::Category::Rvalue;
  }
  return State::Return;
}

Statement::State ExpressionStmt::exec() {
  Variable* v = root_->GetVariable();
  assert(v != nullptr);
  Variable::HandleLife(v);
  return State::Next;
}

Statement::State VariableDefineStmt::exec() {
  Variable* v = CreateVariable(type_name_);
  v->type_name_ = type_name_;
  v->id_name_ = var_name_;
  v->ConstructByExpression(constructors_, Variable::Category::Lvalue);
  Interpreter::instance().GetCurrentContext()->RegisterVariable(v);
  return State::Next;
}
