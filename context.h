#pragma once
#include <unordered_map>
#include <string>
#include "variable.h"

class Context {
public:
  enum class Type { Block, Func, Method, Global };
  Type type_;
  std::unordered_map<std::string, Variable*> vars_;

  explicit Context(Type type) : type_(type) {}
  void RegisterVariable(Variable* v) {
    assert(vars_.find(v->id_name_) == vars_.end());
    vars_[v->id_name_] = v;
  }
  void Clean() {
    for (auto& each : vars_) {
      delete each.second;
    }
  }
  virtual Variable* GetVariableByName(const std::string& name) = 0;
  virtual Variable** GetReturnVar() = 0;
};

class FuncContext : public Context {
public:
  std::string func_name_;
  Variable* return_var_;

  FuncContext() : Context(Type::Func) {}

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

  MethodContext() : Context(Type::Method) {}

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
  explicit BlockContext(Type type) : Context(type) {}

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
