#include "type.h"

static void register_builtin_type(std::unordered_map<std::string, Type>& types) {
  //TODO, 注册内置的类型
  types["int"] = Type("int");
  types["int"].creator_ = [](const std::vector<Variable*>& constructor)->Variable* {
    assert(constructor.size() == 1 && constructor[0]->type_name_ == "int");
    IntVariable* v = new IntVariable;
    v->type_name_ = "int";
    v->val_ = static_cast<IntVariable*>(constructor[0])->val_;
    return v;
  };
  types["string"] = Type("string");
  types["double"] = Type("double");
  types["bool"] = Type("bool");
  types["char"] = Type("char");
  types["void"] = Type("void");
}

TypeSet& TypeSet::instance() {
  static TypeSet obj;
  return obj;
}

TypeSet::TypeSet() {
  register_builtin_type(types_);
}