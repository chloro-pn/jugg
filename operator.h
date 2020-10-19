#pragma once
#include <unordered_map>
#include <functional>
#include "scanner.h"
#include "variable.h"

struct Operator {
  int level_;
  TOKEN token_;
  struct hash_for_maps {
    std::size_t operator()(const std::pair<std::string, std::string > &e) const {
      return std::hash<std::string>()(e.first) + std::hash<std::string>()(e.second);
    }
  };
  std::unordered_map<std::pair<std::string, std::string>, std::string, hash_for_maps> static_maps_;
  Operator(int l, TOKEN t) : level_(l), token_(t) {

  }

  Operator() : level_(-1), token_(TOKEN::INVALID) {

  }

  std::string GetReturnType(const std::string& t1, const std::string& t2) {
    assert(static_maps_.find({ t1, t2 }) != static_maps_.end());
    return static_maps_[{t1, t2}];
  }
};

class OperatorSet {
 public:
  static OperatorSet& instance() {
    static OperatorSet os;
    return os;
  }

  bool find(const TOKEN& token) {
    auto it = operators_.find(token);
    return it != operators_.end();
  }

  Operator& get(const TOKEN& token) {
    return operators_[token];
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