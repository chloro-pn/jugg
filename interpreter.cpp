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
    //�������Ӧ����һ�����͵����ݳ�Ա��
    assert(flag_ == 2);
    const auto& members = static_cast<AbstractVariable*>(v_.top())->members_;
    auto result = std::find_if(members.begin(), members.end(), [&](const Variable*& v)->bool {
      return v->id_name_ == expr->id_name_;
    });
    assert(result != members.end());
    return *result;
  }
  //�����ں����Ĳ����б��Լ���Ӧ�������ڲ���
  else if (flag_ == 1) {
    auto& param_list_ = FuncSet::instance().Get(func_.top()).storage_;
    //�ڲ����б����ҵ���
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
    //ȫ���б�
    return Scopes::instance().GetScopeFromIndex(0)->storage_[expr->id_name_];
  }
}

void Interpreter::Exec() {

}