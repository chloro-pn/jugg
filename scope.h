#pragma once
#include <vector>
#include <unordered_map>
#include "variable.h"

class Scope {
public:
  enum class TYPE {TYPE, FUNC, BLOCK};
  int index_;
  int parent_index_;
  TYPE type_;
  //·ûºÅ±í
  std::unordered_map<std::string, Variable*> vars_;

  bool Find(const std::string& name) const {
    return vars_.find(name) != vars_.end();
  }
};

class Scopes {
public:
  static Scopes& instance();
  void EnterNewScope(Scope::TYPE);
  Scope& GetCurrentScope();
  void LeaveScope();

private:
  Scopes();
  int current_scope_index_;
  std::vector<Scope> scopes_;
};