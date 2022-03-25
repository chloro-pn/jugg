#include "type.h"
#include <cassert>

static void register_builtin_type(std::unordered_map<std::string, Type>& types) {
  types["int"] = Type("int");
  types["string"] = Type("string");
  types["double"] = Type("double");
  types["bool"] = Type("bool");
  types["byte"] = Type("byte");
  types["void"] = Type("void");
}

TypeSet& TypeSet::instance() {
  static TypeSet obj;
  return obj;
}

TypeSet::TypeSet() {
  register_builtin_type(types_);
}