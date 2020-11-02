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
  else if (type_name == "double") {
    return new DoubleVariable;
  }
  else if (type_name == "bool") {
    return new BoolVariable;
  }
  else if (type_name == "byte") {
    return new ByteVariable;
  }
  return new AbstractVariable;
}

//type_name_和id_name_已经被设置
void AbstractVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  Type& type = TypeSet::instance().Get(type_name_.base_type_);
  assert(type.datas_.size() == cs.size());
  for (size_t index = 0; index < cs.size(); ++index) {
    Variable* v = cs[index]->GetVariable();
    assert(v->type_name_ == type.datas_[index].second);
    v->id_name_ = type.datas_[index].first;
    if (v->cate_ == Variable::Category::Lvalue) {
      Variable* tmp = v->Copy(cate);
      tmp->cate_ = cate;
      members_.push_back(tmp);
    }
    else {
      v->cate_ = cate;
      members_.push_back(v);
    }
  }
  cate_ = cate;
}

//type_name和id_name应该已经被设置
void AbstractVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  Type& type = TypeSet::instance().Get(type_name_.base_type_);
  size_t count = type.datas_.size();
  for (size_t i = 0; i < count; ++i) {
    Variable* v = CreateVariable(type.datas_[i].second.base_type_);
    v->type_name_ = type.datas_[i].second;
    v->id_name_ = type.datas_[i].first;
    v->DefaultConstruct(cate);
    members_.push_back(v);
  }
}

Variable* AbstractVariable::Copy(Variable::Category cate) {
  AbstractVariable* result = new AbstractVariable;
  result->type_name_ = type_name_;
  result->id_name_ = id_name_;
  result->cate_ = cate;
  for (auto& each : members_) {
    result->members_.push_back(each->Copy(cate));
  }
  return result;
}

void AbstractVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
  for (auto& each : members_) {
    each->ChangeCategory(cate);
  }
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

void StringVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_.base_type_ == "string");
  val_ = static_cast<StringVariable*>(v)->val_;
  cate_ = cate;
  if (v->cate_ == Variable::Category::Rvalue) {
    delete v;
  }
}

void StringVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  val_ = "";
}

Variable* StringVariable::Copy(Variable::Category cate) {
  StringVariable* result = new StringVariable;
  result->type_name_.base_type_ = "string";
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->val_ = static_cast<StringVariable*>(this)->val_;
  return result;
}

void StringVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void StringVariable::Assign(Variable* v) {
  assert("string" == v->type_name_.base_type_);
  val_ = static_cast<StringVariable*>(v)->val_;
}

Variable* StringVariable::FindMember(const std::string& name) {
  return nullptr;
}

StringVariable::~StringVariable() {
  ;
}

void IntVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_.base_type_ == "int");
  val_ = static_cast<IntVariable*>(v)->val_;
  cate_ = cate;
  if (v->cate_ == Variable::Category::Rvalue) {
    delete v;
  }
}

void IntVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  val_ = 0;
}

Variable* IntVariable::Copy(Variable::Category cate) {
  IntVariable* result = new IntVariable;
  result->type_name_.base_type_ = "int";
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->val_ = static_cast<IntVariable*>(this)->val_;
  return result;
}

void IntVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void IntVariable::Assign(Variable* v) {
  assert("int" == v->type_name_.base_type_);
  val_ = static_cast<IntVariable*>(v)->val_;
}

Variable* IntVariable::FindMember(const std::string& name) {
  return nullptr;
}

IntVariable::~IntVariable() {
  ;
}

void DoubleVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_.base_type_ == "double");
  val_ = static_cast<DoubleVariable*>(v)->val_;
  cate_ = cate;
  if (v->cate_ == Variable::Category::Rvalue) {
    delete v;
  }
}

void DoubleVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  val_ = 0.0;
}

Variable* DoubleVariable::Copy(Variable::Category cate) {
  DoubleVariable* result = new DoubleVariable;
  result->type_name_.base_type_ = "double";
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->val_ = static_cast<DoubleVariable*>(this)->val_;
  return result;
}

void DoubleVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void DoubleVariable::Assign(Variable* v) {
  assert("double" == v->type_name_.base_type_ || "int" == v->type_name_.base_type_);
  val_ = static_cast<DoubleVariable*>(v)->val_;
}

Variable* DoubleVariable::FindMember(const std::string& name) {
  return nullptr;
}

DoubleVariable::~DoubleVariable() {
  ;
}

void BoolVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_.base_type_ == "bool");
  val_ = static_cast<BoolVariable*>(v)->val_;
  cate_ = cate;
  if (v->cate_ == Variable::Category::Rvalue) {
    delete v;
  }
}

void BoolVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  val_ = true;
}

Variable* BoolVariable::Copy(Variable::Category cate) {
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->val_ = static_cast<BoolVariable*>(this)->val_;
  return result;
}

void BoolVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void BoolVariable::Assign(Variable* v) {
  assert("double" == v->type_name_.base_type_);
  val_ = static_cast<BoolVariable*>(v)->val_;
}

Variable* BoolVariable::FindMember(const std::string& name) {
  return nullptr;
}

BoolVariable::~BoolVariable() {
  ;
}

void ByteVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->type_name_.base_type_ == "byte");
  val_ = static_cast<ByteVariable*>(v)->val_;
  cate_ = cate;
  if (v->cate_ == Variable::Category::Rvalue) {
    delete v;
  }
}

void ByteVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  val_ = 0x00;
}

Variable* ByteVariable::Copy(Variable::Category cate) {
  ByteVariable* result = new ByteVariable;
  result->type_name_.base_type_ = "byte";
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->val_ = static_cast<ByteVariable*>(this)->val_;
  return result;
}

void ByteVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void ByteVariable::Assign(Variable* v) {
  assert("byte" == v->type_name_.base_type_);
  val_ = static_cast<ByteVariable*>(v)->val_;
}

Variable* ByteVariable::FindMember(const std::string& name) {
  return nullptr;
}

ByteVariable::~ByteVariable() {
  ;
}

void PointerVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(cs.size() == 1);
  Variable* v = cs[0]->GetVariable();
  assert(v->cate_ == Variable::Category::Lvalue);
  assert(v->type_name_ == type_name_);
  ptr_ = v;
}

void PointerVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  ptr_ = nullptr;
}

Variable* PointerVariable::Copy(Variable::Category cate) {
  PointerVariable* result = new PointerVariable;
  result->type_name_ = type_name_;
  result->id_name_ = id_name_;
  result->cate_ = cate;
  result->ptr_ = ptr_;
  return result;
}

void PointerVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void PointerVariable::Assign(Variable* v) {
  assert(type_name_ == v->type_name_);
  ptr_ = static_cast<PointerVariable*>(v)->ptr_;
}

Variable* PointerVariable::FindMember(const std::string& name) {
  return nullptr;
}

PointerVariable::~PointerVariable() {
  ;
}