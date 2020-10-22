#pragma once
#include <vector>
#include <unordered_map>
#include "ast_node.h"
#include "variable.h"

class Scope {
public:
  int index_;
  int parent_index_;
  std::unordered_map<std::string, std::string> vars_;
  std::unordered_map<std::string, Variable*> storage_;

  virtual ~Scope() = default;
  virtual int Find(const std::string& name) const = 0;
  virtual std::string Get(const std::string& name) = 0;
  virtual void Set(const std::string& var_name, const std::string& type_name) = 0;
};

class BlockScope : public Scope {
public:
  int Find(const std::string& name) const override {
    if (vars_.find(name) != vars_.end()) {
      return index_;
    }
    return -1;
  }

  std::string Get(const std::string& name) override {
    return vars_[name];
  }

  void Set(const std::string& var_name, const std::string& v) override {
    vars_[var_name] = v;
  }
};

class FuncScope : public Scope {
public:
  std::string func_name_;
  
  int Find(const  std::string& name) const override;

  std::string Get(const std::string& name) override;

  void Set(const std::string& var_name, const std::string& v) override;
};

class TypeScope : public Scope {
public:
  std::string type_name_;
  
  int Find(const  std::string& name) const override;

  std::string Get(const std::string& name) override;

  void Set(const std::string& var_name, const std::string& v) override;
};

class MethodScope : public Scope {
public:
  std::string type_name_;
  std::string method_name_;
  
  int Find(const  std::string& name) const override;

  std::string Get(const std::string& name) override;

  void Set(const std::string& var_name, const std::string& v) override;
};

class Scopes {
public:
  static Scopes& instance();
  void EnterBlockScope();
  void EnterFuncScope(const std::string&);
  void EnterTypeScope(const std::string&);
  void EnterMethodScope(const std::string&);
  Scope* GetCurrentScope();
  Scope* GetScopeFromIndex(size_t index);
  size_t VariableStaticBinding(const std::string&);
  std::vector<VariableDefineStmt*>& GetGlobalVariableStmt() {
    return global_variable_stmt_;
  }
  void LeaveScope();

private:
  Scopes();
  int current_scope_index_;
  std::vector<Scope*> scopes_;
  std::vector<VariableDefineStmt*> global_variable_stmt_;
};