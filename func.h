#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include "variable.h"

class BlockStmt;
class Func {
 public:
  std::string func_name_;
  std::vector<std::pair<std::string, std::string>> parameter_type_list_;
  std::string return_type_;
  BlockStmt* block_;
};

class FuncContext {
 public:
  std::string func_name_;
  std::vector<std::pair<std::string, Variable*>> param_;
};

class FuncSet {
 public:
  static FuncSet& instance();

  bool Find(const std::string& func_name) const {
    return funcs_.find(func_name) != funcs_.end();
  }

  Func& Get(const std::string& func_name) {
    return funcs_[func_name];
  }

  void Set(const std::string& func_name, const Func& func) {
    funcs_[func_name] = func;
  }

  void RegisterFunc(const Func& func) {
    assert(Find(func.func_name_) == false);
    Set(func.func_name_, func);
  }

 private:
  std::unordered_map<std::string, Func> funcs_;
  FuncSet();
};