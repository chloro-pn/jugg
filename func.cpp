#include "func.h"

static void register_builtin_func(std::unordered_map<std::string, Func>& funcs) {
  //TODO, ע�����õĺ���
}

FuncSet& FuncSet::instance() {
  static FuncSet obj;
  return obj;
}

FuncSet::FuncSet() {
  register_builtin_func(funcs_);
}