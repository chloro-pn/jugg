#pragma once
#include <vector>
#include <unordered_map>
#include "ast_node.h"

class Scope {
public:
  int index_;
  int parent_index_;
  std::unordered_map<std::string, std::string> vars_;

  virtual ~Scope() = default;
  virtual bool Find(const std::string& name) const = 0;
  virtual std::string Get(const std::string& name) = 0;
  virtual void Set(const std::string& var_name, const std::string& type_name) = 0;
};

class BlockScope : public Scope {
public:
  bool Find(const std::string& name) const override {
    return vars_.find(name) != vars_.end();
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
  
  bool Find(const  std::string& name) const override;

  std::string Get(const std::string& name) override;

  void Set(const std::string& var_name, const std::string& v) override;
};

class TypeScope : public Scope {
public:
  std::string type_name_;
  
  bool Find(const  std::string& name) const override;

  std::string Get(const std::string& name) override;

  void Set(const std::string& var_name, const std::string& v) override;
};

class MethodScope : public Scope {
public:
  std::string type_name_;
  std::string method_name_;
  
  bool Find(const  std::string& name) const override;

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