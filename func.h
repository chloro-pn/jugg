#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "scanner.h"
#include "ast_node.h"

class Func {
 public:
  std::string func_name_;
  std::vector<std::pair<std::string, std::string>> parameter_type_list_;
  std::string return_type_;
  BlockStmt* block_;
};

class FuncSet {
 public:
  static FuncSet& instance();

  bool Find(std::string func_name) const {
    return funcs_.find(func_name) != funcs_.end();
  }

  Func Get(std::string func_name) {
    return funcs_[func_name];
  }

  void Set(std::string func_name, const Func& func) {
    funcs_[func_name] = func;
  }

 private:
  std::unordered_map<std::string, Func> funcs_;
  FuncSet();
};