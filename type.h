#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "func.h"
#include "variable.h"

class Method {
public:
  Variable* caller_;
  std::string method_name_;
  std::vector<std::pair<std::string, Variable*>> parameter_type_list_;
  std::string return_type_;
  std::size_t scope_index_;
  BlockStmt* block_;
};

//TODO
class Type {
 public:
  std::string type_name_;
  // {var_name, type_name}
  std::unordered_map<std::string, std::string> datas_;
  std::unordered_map<std::string, Method> methods_;

  explicit Type(const std::string& str = "") : type_name_(str) {

  }

  bool FindData(const std::string& name) const {
    return datas_.find(name) != datas_.end();
  }

  bool FindMethod(const std::string& name) const {
    return methods_.find(name) != methods_.end();
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