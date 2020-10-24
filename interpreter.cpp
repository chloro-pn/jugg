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
  context_.push(new BlockContext);
}

Variable* Interpreter::FindVariableByName(const std::string& name) {
  std::stack<Context*> c = context_;
  while (c.empty() == false) {
    Context* cur = c.top();
    c.pop();
    Variable* v = cur->GetVariableByName(name);
    if (v != nullptr) {
      return v;
    }
  }
  return nullptr;
}

void Interpreter::Exec() {
  //全局变量的构造
  for (auto& each : global_var_) {
    Variable* v = CreateVariable(each->type_name_);
    v->id_name_ = each->var_name_;
    v->type_name_ = each->type_name_;
    v->cate_ = Variable::Category::Lvalue;
    v->ConstructByExpression(each->constructors_);
    GetCurrentContext()->vars_[v->id_name_] = v;
  }
  CallFunc("main", {});
}

Variable* Interpreter::CallFunc(const std::string& func_name, const std::vector<Variable*>& variables) {
  assert(FuncSet::instance().Find(func_name) == true);
  Func& func = FuncSet::instance().Get(func_name);
  FuncContext* fc = new FuncContext;
  fc->func_name_ = func_name;
  //main函数没有参数传入
  assert(func.parameter_type_list_.size() == variables.size());
  for (int i = 0; i < variables.size(); ++i) {
    assert(func.parameter_type_list_[i].second == variables[i]->type_name_);
    fc->vars_[func.parameter_type_list_[i].first] = variables[i]->Copy();
  }
  //这里需要一种机制，将fc的return_var_注册给func的block语句中的return语句。
  context_.push(fc);
  Statement::State s = func.block_->exec();
  assert(s == Statement::State::Return);
  for (auto& each : fc->vars_) {
    delete each.second;
  }
  context_.pop();
  //函数返回值是一个右值。
  fc->return_var_->cate_ = Variable::Category::Rvalue;
  return fc->return_var_;
}

void Interpreter::RegisterGlobalVariable(VariableDefineStmt* v) {
  auto it = std::find_if(global_var_.begin(), global_var_.end(), [v](VariableDefineStmt* each)->bool {
    return v->var_name_ == each->var_name_;
  });
  assert(it == global_var_.end());
  global_var_.push_back(v);
}