#include "interpreter.h"
#include "type.h"
#include "scope.h"
#include "ast_node.h"
#include <algorithm>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

Variable* Interpreter::FindVariableByIdexpr(const IdExpr* expr) {
  if (expr->scope_index_ == TypeSet::instance().Get(expr->type_name_).scope_index_) {
    //这个变量应该是一个类型的数据成员。
    assert(flag_ == 2);
    const auto& members = static_cast<AbstractVariable*>(v_.top())->members_;
    auto result = std::find_if(members.begin(), members.end(), [&](const Variable*& v)->bool {
      return v->id_name_ == expr->id_name_;
    });
    assert(result != members.end());
    return *result;
  }
  //否则在函数的参数列表以及对应作用域内查找
  else if (flag_ == 1) {
    auto& param_list_ = FuncSet::instance().Get(func_.top()).storage_;
    //在参数列表中找到了
    if (param_list_.find(expr->id_name_) != param_list_.end()) {
      return param_list_[expr->id_name_];
    }
    else {
      int scope_index = FuncSet::instance().Get(func_.top()).scope_index_;
      return Scopes::instance().GetScopeFromIndex(scope_index)->storage_[expr->id_name_];
    }
  }
  else {
    assert(flag_ == 0);
    //全局列表
    return Scopes::instance().GetScopeFromIndex(0)->storage_[expr->id_name_];
  }
}

void Interpreter::Exec() {

}