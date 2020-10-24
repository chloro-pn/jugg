#pragma once
#include <unordered_map>
#include <string>
#include "variable.h"

class Context {
public:
  std::unordered_map<std::string, Variable*> vars_;

  virtual Variable* GetVariableByName(const std::string& name) = 0;
};

class FuncContext : public Context {
public:
  std::string func_name_;
  Variable* return_var_;

  Variable* GetVariableByName(const std::string& name) override {
    if (vars_.find(name) != vars_.end()) {
      return vars_[name];
    }
    return nullptr;
  }
};

class MethodContext : public Context {
public:
  Variable* obj_;
  std::string method_name_;
  Variable* return_var_;

  Variable* GetVariableByName(const std::string& name) override {
    if (vars_.find(name) != vars_.end()) {
      return vars_[name];
    }
    Variable* v = obj_->FindMember(name);
    return v;
  }
};

class BlockContext : public Context {
public:
  Variable* GetVariableByName(const std::string& name) override {
    if (vars_.find(name) != vars_.end()) {
      return vars_[name];
    }
    return nullptr;
  }
};
