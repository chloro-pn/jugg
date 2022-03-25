#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include "comprehensive_type.h"
#include "variable.h"
#include "ast_node.h"

class BlockStmt;
class Func {
 public:
  std::string func_name_;
  // {id_name, type_name}
  std::vector<std::pair<std::string, ComprehensiveType>> parameter_type_list_;
  ComprehensiveType return_type_;
  BlockStmt* block_;
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

  Statement::State CallFunc(const std::string& func_name) {
    assert(Find(func_name) == true);
    return Get(func_name).block_->exec();
  }

 private:
  std::unordered_map<std::string, Func> funcs_;
  FuncSet();
};