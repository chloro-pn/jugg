#include "interpreter.h"
#include "type.h"
#include "ast_node.h"
#include "variable.h"
#include <algorithm>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

//初始情况下在全局作用域。
Interpreter::Interpreter() {
  flag_.push(0);
}

Variable* Interpreter::FindVariableByIdexpr(const IdExpr* expr) {
  return nullptr;
}

void Interpreter::Exec() {
  //全局变量的构造
  for (auto& each : global_var_) {
    Variable* v = CreateVariable(each->type_name_);
    v->id_name_ = each->var_name_;
    v->type_name_ = each->type_name_;
    v->ConstructByExpression(each->constructors_);
  }
  assert(FuncSet::instance().Find("main") == true);
  Func& main = FuncSet::instance().Get("main");
  
}

void Interpreter::CallFunc(const std::string& func_name) {
  assert(FuncSet::instance().Find(func_name) == true);
  Func& main = FuncSet::instance().Get(func_name);
  FuncContext fc;
  fc.func_name_ = func_name;
  //main函数没有参数传入
  assert(main.parameter_type_list_.size() == 0);
  func_.push(fc);
  flag_.push(1);
  main.block_->exec();
}

void Interpreter::RegisterGlobalVariable(VariableDefineStmt* v) {
  auto it = std::find_if(global_var_.begin(), global_var_.end(), [v](VariableDefineStmt* each)->bool {
    return v->var_name_ == each->var_name_;
  });
  assert(it == global_var_.end());
  global_var_.push_back(v);
}