#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "func.h"

//TODO
class Type {
 public:
  std::string type_name_;
  // std::pair<type_name, variable_name>.
  std::vector<std::pair<std::string, std::string>> data_members_;
  std::vector<Func> methods_;

  explicit Type(const std::string& str = "") : type_name_(str) {

  }
};

class TypeSet {
public:
  static TypeSet& instance();

  bool Find(std::string type_name) const {
    return types_.find(type_name) != types_.end();
  }

  Type Get(std::string type_name) {
    return types_[type_name];
  }

  void Set(std::string type_name, const Type& type) {
    types_[type_name] = type;
  }

private:
  std::unordered_map<std::string, Type> types_;
  TypeSet();
};