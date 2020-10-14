#pragma once
#include <unordered_map>
#include "scanner.h"

struct Operator {
  int level_;
  TOKEN token_;

  Operator(int l, TOKEN t) : level_(l), token_(t) {

  }

  Operator() : level_(-1), token_(TOKEN::INVALID) {

  }
};

class OperatorSet {
 public:
  static OperatorSet& instance() {
    static OperatorSet os;
    return os;
  }

  bool find(TOKEN token) {
    auto it = operators_.find(token);
    return it != operators_.end();
  }

  int GetLevel(TOKEN token) {
    if (find(token) == false) {
      return -1;
    }
    return operators_[token].level_;
  }

 private:
  OperatorSet();

  std::unordered_map<TOKEN, Operator> operators_;
};