#pragma once
#include <unordered_map>
#include <string>
#include "variable.h"
#include "func.h"
#include "type.h"

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
  virtual void InitVars(const std::vector<Variable*>& vars) = 0;
  virtual Variable* GetVariableByName(const std::string& name) = 0;
  virtual Variable** GetReturnVar() = 0;
  virtual ~Context() {

  }
};

class FuncContext : public Context {
public:
  std::string func_name_;
  Variable* return_var_;

  explicit FuncContext(const std::string& func_name) : Context(Type::Func), func_name_(func_name) {
    assert(FuncSet::instance().Find(func_name_) == true);
  }

  void InitVars(const std::vector<Variable*>& vars) override {
    Func& func = FuncSet::instance().Get(func_name_);
    assert(func.parameter_type_list_.size() == vars.size());
    for (size_t i = 0; i < vars.size(); ++i) {
      assert(func.parameter_type_list_[i].second == vars[i]->type_name_);
      //注意应该使用函数参数的名字。
      vars[i]->id_name_ = func.parameter_type_list_[i].first;
      RegisterVariable(vars[i]);
    }
  }

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

  MethodContext(Variable* obj, const std::string& method_name) : Context(Type::Method), obj_(obj), method_name_(method_name) {}

  void InitVars(const std::vector<Variable*>& vars) override {
    ::Type& type = TypeSet::instance().Get(obj_->type_name_.base_type_);
    Method& method = type.GetMethod(method_name_);

    assert(method.parameter_type_list_.size() == vars.size());
    for (size_t i = 0; i < vars.size(); ++i) {
      assert(method.parameter_type_list_[i].second == vars[i]->type_name_);
      //注意应该使用函数参数的名字。
      vars[i]->id_name_ = method.parameter_type_list_[i].first;
      RegisterVariable(vars[i]);
    }
  }

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

  void InitVars(const std::vector<Variable*>& vars) override {
    assert(vars.size() == 0);
  }

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
