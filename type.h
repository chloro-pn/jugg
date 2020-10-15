#pragma once
#include <string>
#include <unordered_map>

//TODO
class Type {
 public:
  std::string type_name_;
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