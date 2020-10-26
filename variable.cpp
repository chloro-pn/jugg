#include "variable.h"
#include "type.h"
#include "ast_node.h"

Variable* CreateVariable(const std::string& type_name) {
  if (type_name == "string") {
    return new StringVariable;
  }
  else if (type_name == "int") {
    return new IntVariable;
  }
  return new AbstractVariable;
}

//type_name_和id_name_已经被设置
void AbstractVariable::ConstructByExpression(const std::vector<Expression*>& cs) {
  Type& type = TypeSet::instance().Get(type_name_);
  assert(type.datas_.size() == cs.size());
  for (size_t index = 0; index < cs.size(); ++index) {
    Variable* v = cs[index]->GetVariable();
    assert(v->type_name_ == type.datas_[index].second);
    v->id_name_ = type.datas_[index].first;
    if (v->cate_ == Variable::Category::Lvalue) {
      Variable* tmp = v->Copy();
      tmp->cate_ = Variable::Category::Rvalue;
      members_.push_back(tmp);
    }
    else {
      v->cate_ = Variable::Category::Rvalue;
      members_.push_back(v);
    }
  }
}

Variable* AbstractVariable::Copy() {
  AbstractVariable* result = new AbstractVariable;
  result->type_name_ = type_name_;
  result->id_name_ = id_name_;
  for (auto& each : members_) {
    result->members_.push_back(each->Copy());
  }
  return result;
}

void AbstractVariable::Assign(Variable* v) {
  assert(type_name_ == v->type_name_);
  for (int i = 0; i < members_.size(); ++i) {
    members_[i]->Assign(static_cast<AbstractVariable*>(v)->members_[i]);
  }
}

Variable* AbstractVariable::FindMember(const std::string& name) {
  for (auto& each : members_) {
    if (each->id_name_ == name) {
      return each;
    }
  }
  return nullptr;
}

AbstractVariable::~AbstractVariable() {
  for (auto& each : members_) {
    delete each;
  }
}

void StringVariable::ConstructByExpression(const std::vector<Expression*>& cs) {
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_ == "string");
  val_ = static_cast<StringVariable*>(v)->val_;
  delete v;
}

Variable* StringVariable::Copy() {
  StringVariable* result = new StringVariable;
  result->type_name_ = "string";
  result->id_name_ = id_name_;
  result->val_ = static_cast<StringVariable*>(this)->val_;
  return result;
}

void StringVariable::Assign(Variable* v) {
  assert("string" == v->type_name_);
  val_ = static_cast<StringVariable*>(v)->val_;
}

Variable* StringVariable::FindMember(const std::string& name) {
  return nullptr;
}

StringVariable::~StringVariable() {
  ;
}

void IntVariable::ConstructByExpression(const std::vector<Expression*>& cs) {
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_ == "int");
  val_ = static_cast<IntVariable*>(v)->val_;
  delete v;
}

Variable* IntVariable::Copy() {
  IntVariable* result = new IntVariable;
  result->type_name_ = "int";
  result->id_name_ = id_name_;
  result->val_ = static_cast<IntVariable*>(this)->val_;
  return result;
}

void IntVariable::Assign(Variable* v) {
  assert("int" == v->type_name_);
  val_ = static_cast<IntVariable*>(v)->val_;
}

Variable* IntVariable::FindMember(const std::string& name) {
  return nullptr;
}

IntVariable::~IntVariable() {
  ;
}