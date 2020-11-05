#include "func.h"
#include <cassert>

static void register_builtin_func(std::unordered_map<std::string, Func>& funcs) {
  //TODO, ×¢²áÄÚÖÃµÄº¯Êı
    assert(funcs.empty() == true);
}

FuncSet& FuncSet::instance() {
  static FuncSet obj;
  return obj;
}

FuncSet::FuncSet() {
  register_builtin_func(funcs_);
}
