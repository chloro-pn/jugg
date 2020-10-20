#pragma once
#include <vector>
#include <unordered_map>
#include "variable.h"
#include "ast_node.h"

class Scope {
public:
  int index_;
  int parent_index_;
  std::unordered_map<std::string, Variable*> vars_;

  virtual ~Scope() = default;
  virtual bool Find(const std::string& name) const = 0;
  virtual Variable* Get(const std::string& name) = 0;
  virtual void Set(const std::string var_name, Variable* v) = 0;
};

class BlockScope : public Scope {
public:
  bool Find(const std::string& name) const override {
    return vars_.find(name) != vars_.end();
  }

  Variable* Get(const std::string& name) override {
    return vars_[name];
  }

  void Set(const std::string var_name, Variable* v) override {

  }
};

class FuncScope : public Scope {
public:
  std::string func_name_;
  
  bool Find(const  std::string& name) const override {
    //TODO
  }

  Variable* Get(const std::string& name) override {
    //TODO
  }

  void Set(const std::string var_name, Variable* v) override {

  }
};

class TypeScope : public Scope {
public:
  std::string type_name_;
  
  bool Find(const  std::string& name) const override {
    //TODO
  }

  Variable* Get(const std::string& name) override {
    //TODO
  }

  void Set(const std::string var_name, Variable* v) override {

  }
};

class MethodScope : public Scope {
public:
  std::string type_name_;
  std::string method_name_;
  
  bool Find(const  std::string& name) const override {
    //TODO
  }

  Variable* Get(const std::string& name) override {
    //TODO
  }

  void Set(const std::string var_name, Variable* v) override {

  }
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
  Variable* VariableStaticBinding(const std::string& var_name);

private:
  Scopes();
  int current_scope_index_;
  std::vector<Scope*> scopes_;
  std::vector<VariableDefineStmt*> global_variable_stmt_;
};