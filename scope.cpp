#include "scope.h"
#include "variable.h"
#include <cassert>

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

Variable* Scopes::VariableStaticBinding(const std::string& var_name) {
  size_t index = GetCurrentScope()->index_;
  while (true) {
    if (scopes_[index]->Find(var_name) == true) {
      return scopes_[index]->Get(var_name);
    }
    assert(index > 0);
    index = scopes_[index]->parent_index_;
  }
  // never attach there.
  return nullptr;
}

Scopes::Scopes() {
  BlockScope* global_scope = new BlockScope;
  global_scope->index_ = 0;
  global_scope->parent_index_ = -1;
  scopes_.push_back(global_scope);
  current_scope_index_ = 0;
}