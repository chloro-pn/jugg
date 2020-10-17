#include "scope.h"
#include <cassert>

Scopes& Scopes::instance() {
  static Scopes obj;
  return obj;
}

void Scopes::EnterNewScope(Scope::TYPE type) {
  Scope scope;
  scope.index_ = scopes_.size();
  scope.parent_index_ = current_scope_index_;
  scope.type_ = type;
  scopes_.push_back(scope);
  current_scope_index_ = scope.index_;
}

Scope& Scopes::GetCurrentScope() {
  assert(current_scope_index_ >= 0 && current_scope_index_ < scopes_.size());
  return scopes_[current_scope_index_];
}

void Scopes::LeaveScope() {
  assert(current_scope_index_ > 0);
  current_scope_index_ = scopes_[current_scope_index_].parent_index_;
}

Scopes::Scopes() {
  Scope global_scope;
  global_scope.index_ = 0;
  global_scope.parent_index_ = -1;
  global_scope.type_ = Scope::TYPE::BLOCK;
  scopes_.push_back(global_scope);
  current_scope_index_ = 0;
}