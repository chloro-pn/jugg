#pragma once
#include <vector>

class Scope {
public:
  enum class TYPE {TYPE, FUNC, BLOCK};
  int index_;
  int parent_index_;
  TYPE type_;
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