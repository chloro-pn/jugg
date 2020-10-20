#include "scope.h"
#include "type.h"
#include <cassert>
#include <algorithm>

bool FuncScope::Find(const std::string& name) const {
  Func& func = FuncSet::instance().Get(name);
  if (func.parameter_type_list_.find(name) != func.parameter_type_list_.end()) {
    return true;
  }
  return vars_.find(name) != vars_.end();
}

std::string FuncScope::Get(const std::string& name) {
  Func& func = FuncSet::instance().Get(func_name_);
  if (func.parameter_type_list_.find(name) != func.parameter_type_list_.end()) {
    return func.parameter_type_list_[name];
  }
  return vars_[name];
}

//这个接口仅注册局部变量，不注册参数列表
void FuncScope::Set(const std::string& var_name, const std::string& type_name) {
  vars_[var_name] = type_name;
}

bool TypeScope::Find(const std::string& name) const {
  return vars_.find(name) != vars_.end();
}

std::string TypeScope::Get(const std::string& name) {
  return vars_[name];
}

void TypeScope::Set(const std::string& var_name, const std::string& v) {
  vars_[var_name] = v;
}

bool MethodScope::Find(const  std::string& name) const {
  Type& type = TypeSet::instance().Get(type_name_);
  Method& method = type.methods_[method_name_];
  if (method.parameter_type_list_.find(name) != method.parameter_type_list_.end()) {
    return true;
  }
  if (type.datas_.find(name) != type.datas_.end()) {
    return true;
  }
  return vars_.find(name) != vars_.end();
}

std::string MethodScope::Get(const std::string& name) {
  Type& type = TypeSet::instance().Get(type_name_);
  Method& method = type.methods_[method_name_];
  if (method.parameter_type_list_.find(name) != method.parameter_type_list_.end()) {
    return method.parameter_type_list_[name];
  }
  if (type.datas_.find(name) != type.datas_.end()) {
    return type.datas_[name];
  }
  return vars_[name];
}

//这个接口仅注册局部变量，不注册参数列表和类型的成员列表
void MethodScope::Set(const std::string& var_name, const std::string& v) {
  vars_[var_name] = v;
}

Scopes& Scopes::instance() {
  static Scopes obj;
  return obj;
}

void Scopes::EnterBlockScope() {
  BlockScope* scope = new BlockScope;
  scope->index_ = scopes_.size();
  scope->parent_index_ = current_scope_index_;
  scopes_.push_back(scope);
  current_scope_index_ = scope->index_;
}

void Scopes::EnterFuncScope(const std::string& func_name) {
  FuncScope* scope = new FuncScope;
  scope->index_ = scopes_.size();
  scope->parent_index_ = current_scope_index_;
  scope->func_name_ = func_name;
  scopes_.push_back(scope);
  current_scope_index_ = scope->index_;
}

void Scopes::EnterTypeScope(const std::string& type_name) {
  TypeScope* scope = new TypeScope;
  scope->index_ = scopes_.size();
  scope->parent_index_ = current_scope_index_;
  scope->type_name_ = type_name;
  scopes_.push_back(scope);
  current_scope_index_ = scope->index_;
}

void Scopes::EnterMethodScope(const std::string& method_name) {
  MethodScope* scope = new MethodScope;
  scope->index_ = scopes_.size();
  scope->parent_index_ = current_scope_index_;
  scope->method_name_ = method_name;
  scopes_.push_back(scope);
  current_scope_index_ = scope->index_;
}

Scope* Scopes::GetCurrentScope() {
  assert(current_scope_index_ >= 0 && current_scope_index_ < scopes_.size());
  return scopes_[current_scope_index_];
}

void Scopes::LeaveScope() {
  assert(current_scope_index_ > 0);
  current_scope_index_ = scopes_[current_scope_index_]->parent_index_;
}

Scopes::Scopes() {
  BlockScope* global_scope = new BlockScope;
  global_scope->index_ = 0;
  global_scope->parent_index_ = -1;
  scopes_.push_back(global_scope);
  current_scope_index_ = 0;
}