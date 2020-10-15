#include "type.h"

static void register_builtin_type(std::unordered_map<std::string, Type>& types) {
  //TODO, 注册内置的类型
}

TypeSet& TypeSet::instance() {
  static TypeSet obj;
  return obj;
}

TypeSet::TypeSet() {
  register_builtin_type(types_);
}