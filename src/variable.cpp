#include "variable.h"
#include "type.h"
#include "ast_node.h"

Variable* CreateVariable(const ComprehensiveType& type_name) {
  if (type_name.IsBaseType() == false) {
    assert(false);
  }
  if (type_name.type_ == ComprehensiveType::TYPE::ARRAY) {
    auto result = new ArrayVariable;
    result->ele_num_ = type_name.array_num_;
    return result;
  }
  if (type_name.IsBaseType("string")) {
    return new StringVariable;
  }
  else if (type_name.IsBaseType("int")) {
    return new IntVariable;
  }
  else if (type_name.IsBaseType("double")) {
    return new DoubleVariable;
  }
  else if (type_name.IsBaseType("bool")) {
    return new BoolVariable;
  }
  else if (type_name.IsBaseType("byte")) {
    return new ByteVariable;
  }
  return new AbstractVariable;
}

void AbstractVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(type_name_.IsBaseType());
  Type& type = TypeSet::instance().Get(type_name_.base_type_);
  assert(type.datas_.size() == cs.size());
  for (size_t index = 0; index < cs.size(); ++index) {
    Variable* v = cs[index]->GetVariable();
    assert(v != nullptr && v->type_name_ == type.GetNthDataMemberType(index));
    if (v->IsLValue()) {
      Variable* tmp = v->Copy(cate);
      tmp->id_name_ = type.GetNthDataMemberName(index);
      members_.push_back(tmp);
    }
    else {
      v->ChangeCategory(cate);
      v->id_name_ = type.GetNthDataMemberName(index);
      members_.push_back(v);
    }
  }
  cate_ = cate;
}

void AbstractVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  assert(type_name_.IsBaseType());
  Type& type = TypeSet::instance().Get(type_name_.base_type_);
  size_t count = type.datas_.size();
  for (size_t i = 0; i < count; ++i) {
    Variable* v = CreateVariable(type.GetNthDataMemberType(i));
    v->type_name_ = type.GetNthDataMemberType(i);
    v->id_name_ = type.GetNthDataMemberName(i);
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
  for (size_t i = 0; i < members_.size(); ++i) {
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

void ArrayVariable::ConstructByExpression(const std::vector<Expression*>& cs, Variable::Category cate) {
  if (cs.size() == 0) {
    DefaultConstruct(cate);
    return;
  }
  assert(type_name_.IsBaseType());
  assert(ele_num_ == cs.size());
  for (size_t index = 0; index < cs.size(); ++index) {
    Variable* v = cs[index]->GetVariable();
    assert(v != nullptr && v->type_name_ == type_name_);
    if (v->IsLValue()) {
      Variable* tmp = v->Copy(cate);
      tmp->id_name_ = "tmp";
      members_.push_back(tmp);
    }
    else {
      v->ChangeCategory(cate);
      v->id_name_ = "tmp";
      members_.push_back(v);
    }
  }
  cate_ = cate;
}

void ArrayVariable::DefaultConstruct(Variable::Category cate) {
  cate_ = cate;
  assert(type_name_.IsBaseType());
  size_t count = ele_num_;
  for (size_t i = 0; i < count; ++i) {
    Variable* v = CreateVariable(type_name_);
    v->type_name_ = type_name_;
    v->id_name_ = "tmp";
    v->DefaultConstruct(cate);
    members_.push_back(v);
  }
}

Variable* ArrayVariable::Copy(Variable::Category cate) {
  ArrayVariable* result = new ArrayVariable;
  result->type_name_ = type_name_;
  result->id_name_ = id_name_;
  result->cate_ = cate;
  for (auto& each : members_) {
    result->members_.push_back(each->Copy(cate));
  }
  return result;
}

void ArrayVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
  for (auto& each : members_) {
    each->ChangeCategory(cate);
  }
}

void ArrayVariable::Assign(Variable* v) {
  assert(type_name_ == v->type_name_);
  for (size_t i = 0; i < members_.size(); ++i) {
    // todo 增强类型检测
    members_[i]->Assign(static_cast<ArrayVariable*>(v)->members_[i]);
  }
}

Variable* ArrayVariable::FindMember(const std::string& name) {
  return nullptr;
}

ArrayVariable::~ArrayVariable() {
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
  assert(v != nullptr && v->type_name_.IsBaseType("string"));
  val_ = VariableCast<StringVariable>(v)->val_;
  cate_ = cate;
  Variable::HandleLife(v);
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
  result->val_ = val_;
  return result;
}

void StringVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void StringVariable::Assign(Variable* v) {
  assert(v->type_name_.IsBaseType("string"));
  val_ = VariableCast<StringVariable>(v)->val_;
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
  assert(v != nullptr && v->type_name_.IsBaseType("int"));
  val_ = VariableCast<IntVariable>(v)->val_;
  cate_ = cate;
  Variable::HandleLife(v);
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
  result->val_ = val_;
  return result;
}

void IntVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void IntVariable::Assign(Variable* v) {
  assert(v->type_name_.IsBaseType("int"));
  val_ = VariableCast<IntVariable>(v)->val_;
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
  assert(v != nullptr && v->type_name_.IsBaseType("double"));
  val_ = VariableCast<DoubleVariable>(v)->val_;
  cate_ = cate;
  Variable::HandleLife(v);
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
  result->val_ = val_;
  return result;
}

void DoubleVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void DoubleVariable::Assign(Variable* v) {
  assert(v->type_name_.IsBaseType("double"));
  val_ = VariableCast<DoubleVariable>(v)->val_;
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
  assert(v != nullptr && v->type_name_.IsBaseType("bool"));
  val_ = VariableCast<BoolVariable>(v)->val_;
  cate_ = cate;
  Variable::HandleLife(v);
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
  result->val_ = val_;
  return result;
}

void BoolVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void BoolVariable::Assign(Variable* v) {
  assert(v->type_name_.IsBaseType("bool"));
  val_ = VariableCast<BoolVariable>(v)->val_;
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
  assert(v != nullptr && v->type_name_.IsBaseType("byte"));
  val_ = VariableCast<ByteVariable>(v)->val_;
  cate_ = cate;
  Variable::HandleLife(v);
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
  result->val_ = val_;
  return result;
}

void ByteVariable::ChangeCategory(Variable::Category cate) {
  cate_ = cate;
}

void ByteVariable::Assign(Variable* v) {
  assert(v->type_name_.IsBaseType("byte"));
  val_ = VariableCast<ByteVariable>(v)->val_;
}

Variable* ByteVariable::FindMember(const std::string& name) {
  return nullptr;
}

ByteVariable::~ByteVariable() {
  ;
}