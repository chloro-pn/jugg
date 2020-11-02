#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include "variable.h"
#include "func.h"
#include "comprehensive_type.h"

class Method {
public:
  std::string type_name_;
  std::string method_name_;
  std::vector<std::pair<std::string, ComprehensiveType>> parameter_type_list_;
  ComprehensiveType return_type_;
  BlockStmt* block_;
};

//TODO
class Type {
 public:
  std::string type_name_;
  // {var_name, type_name}
  std::vector<std::pair<std::string, ComprehensiveType>> datas_;
  std::unordered_map<std::string, Method> methods_;

  explicit Type(const std::string& str = "") : type_name_(str) {

  }

  bool FindMethod(const std::string& name) const {
    return methods_.find(name) != methods_.end();
  }

  void RegisterData(const std::string& var_name, const ComprehensiveType& type_name) {
    auto it = std::find_if(datas_.begin(), datas_.end(), [&](const std::pair<std::string, ComprehensiveType>& each)->bool {
      return each.first == var_name;
    });
    assert(it == datas_.end());
    datas_.push_back({ var_name, type_name });
  }

  void RegisterMethod(const Method& method) {
    assert(FindMethod(method.method_name_) == false);
    methods_[method.method_name_] = method;
  }
};

class TypeSet {
public:
  static TypeSet& instance();

  bool Find(const std::string& type_name) const {
    return types_.find(type_name) != types_.end();
  }

  Type& Get(const std::string& type_name) {
    return types_[type_name];
  }

  void Set(const std::string& type_name, const Type& type) {
    types_[type_name] = type;
  }

  void RegisterType(const Type& type) {
    assert(Find(type.type_name_) == false);
    Set(type.type_name_, type);
  }

private:
  std::unordered_map<std::string, Type> types_;
  TypeSet();
};