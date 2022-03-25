#pragma once
#include <unordered_map>
#include <functional>
#include "token.h"
#include "variable.h"

struct BinaryOperator {
  int level_;
  TOKEN token_;
  struct hash_for_maps {
    std::size_t operator()(const std::pair<std::string, std::string> &e) const {
      return std::hash<std::string>()(e.first) + std::hash<std::string>()(e.second);
    }
  };
  std::unordered_map<std::pair<std::string, std::string>, std::function<Variable*(Variable*, Variable*)>, hash_for_maps> op_funcs_;
  BinaryOperator(int l, TOKEN t) : level_(l), token_(t) {

  }

  BinaryOperator() : level_(-1), token_(TOKEN::INVALID) {

  }

  Variable* Handle(Variable* v1, Variable* v2) {
    assert(FindOpFuncs(v1->type_name_.base_type_, v2->type_name_.base_type_));
    return op_funcs_[{v1->type_name_.base_type_, v2->type_name_.base_type_}](v1, v2);
  }

  bool FindOpFuncs(const std::string& t1, const std::string& t2) const {
    return op_funcs_.find({ t1, t2 }) != op_funcs_.end();
  }
};

struct UnaryOperator {
  TOKEN token_;
  std::function<Variable*(Variable*)> func_;
};

class OperatorSet {
 public:
  static OperatorSet& instance() {
    static OperatorSet os;
    return os;
  }

  Variable* HandleBinary(TOKEN token, Variable* v1, Variable* v2) {
    assert(FindBinary(token) == true);
    return boperators_[token].Handle(v1, v2);
  }

  int GetLevel(TOKEN token) {
    assert(FindBinary(token) == true);
    return boperators_[token].level_;
  }

  Variable* HandleUnary(TOKEN token, Variable* v) {
    assert(FindUnary(token) == true);
    return uoperators_[token].func_(v);
  }

  bool Find(const TOKEN& token) {
    return FindBinary(token) || FindUnary(token);
  }

  bool FindBinary(const TOKEN& token) {
    auto it = boperators_.find(token);
    return it != boperators_.end();
  }

  bool FindUnary(const TOKEN& token) {
    auto it = uoperators_.find(token);
    return it != uoperators_.end();
  }

 private:
  OperatorSet();
  std::unordered_map<TOKEN, BinaryOperator> boperators_;
  std::unordered_map<TOKEN, UnaryOperator> uoperators_;
};