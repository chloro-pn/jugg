#pragma once
#include <unordered_map>
#include <string>
#include "variable.h"

class Context {
public:
  std::unordered_map<std::string, Variable*> vars_;

  virtual Variable* GetVariableByName(const std::string& name) = 0;
  virtual Variable** GetReturnVar() = 0;
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

  Variable** GetReturnVar() override {
    return &return_var_;
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

  Variable** GetReturnVar() override {
    return &return_var_;
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

  Variable** GetReturnVar() override {
    return nullptr;
  }
};
