#include "interpreter.h"
#include "type.h"
#include "ast_node.h"
#include "variable.h"
#include <algorithm>
#include <iostream>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

//��ʼ�������ȫ��������
Interpreter::Interpreter() {
  context_.push(new BlockContext);
  inner_func_["print"] = [](FuncContext* fc)->void {
    fc->return_var_ = new VoidVariable;
    fc->return_var_->id_name_ = "void";
    for (auto& each : fc->vars_) {
      if (each.second->type_name_ == "int") {
        std::cout << static_cast<IntVariable*>(each.second)->val_;
      }
      else if (each.second->type_name_ == "string") {
        std::cout << static_cast<StringVariable*>(each.second)->val_;
      }
      else {
        assert(false);
      }
    }
  };
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
  //ȫ�ֱ����Ĺ���
  for (auto& each : global_var_) {
    Variable* v = CreateVariable(each->type_name_);
    v->id_name_ = each->var_name_;
    v->type_name_ = each->type_name_;
    v->ConstructByExpression(each->constructors_, Variable::Category::Lvalue);
    GetCurrentContext()->vars_[v->id_name_] = v;
  }
  Variable* v = CallFunc("main", {});
  assert(v->type_name_ == "void");
  delete v;
  assert(context_.size() == 1);
  //ȫ�ֱ���������
  for (auto& each : GetCurrentContext()->vars_) {
    delete each.second;
  }
}

// variables�Ѿ��Ǻ����������ڵı���
Variable* Interpreter::CallFunc(const std::string& func_name, const std::vector<Variable*>& variables) {
  if (inner_func_.find(func_name) != inner_func_.end()) {
    FuncContext* fc = new FuncContext;
    fc->func_name_ = func_name;
    //main����û�в�������
    for (int i = 0; i < variables.size(); ++i) {
      fc->vars_[variables[i]->id_name_] = variables[i];
    }
    inner_func_[func_name](fc);
    for (auto& each : fc->vars_) {
      delete each.second;
    }
    Variable* result = fc->return_var_;
    delete fc;
    return result;
  }
  assert(FuncSet::instance().Find(func_name) == true);
  Func& func = FuncSet::instance().Get(func_name);
  FuncContext* fc = new FuncContext;
  fc->func_name_ = func_name;
  //main����û�в�������
  assert(func.parameter_type_list_.size() == variables.size());
  for (int i = 0; i < variables.size(); ++i) {
    assert(func.parameter_type_list_[i].second == variables[i]->type_name_);
    //ע��Ӧ��ʹ�ú������������֡�
    fc->vars_[func.parameter_type_list_[i].first] = variables[i];
    fc->vars_[func.parameter_type_list_[i].first]->id_name_ = func.parameter_type_list_[i].first;
  }
  //������Ҫһ�ֻ��ƣ���fc��return_var_ע���func��block����е�return��䡣
  func.block_->RegisterReturnVar(fc->return_var_);
  context_.push(fc);
  Statement::State s = func.block_->exec();
  assert(s == Statement::State::Return);
  for (auto& each : fc->vars_) {
    delete each.second;
  }
  Variable* result = fc->return_var_;
  context_.pop();
  delete fc;
  //��������ֵ��һ����ֵ��
  return result;
}

Variable* Interpreter::CallMethod(Variable* obj, const std::string& method_name, const std::vector<Variable*>& variables) {
  Type& type = TypeSet::instance().Get(obj->type_name_);
  assert(type.methods_.find(method_name) != type.methods_.end());
  Method& method = type.methods_[method_name];
  MethodContext* mc = new MethodContext;
  mc->method_name_ = method_name;
  mc->obj_ = obj;
  assert(method.parameter_type_list_.size() == variables.size());
  for (int i = 0; i < variables.size(); ++i) {
    assert(method.parameter_type_list_[i].second == variables[i]->type_name_);
    //ע��Ӧ��ʹ�ú������������֡�
    mc->vars_[method.parameter_type_list_[i].first] = variables[i];
    mc->vars_[method.parameter_type_list_[i].first]->id_name_ = method.parameter_type_list_[i].first;
  }
  //������Ҫһ�ֻ��ƣ���fc��return_var_ע���func��block����е�return��䡣
  method.block_->RegisterReturnVar(mc->return_var_);
  context_.push(mc);
  Statement::State s = method.block_->exec();
  assert(s == Statement::State::Return);
  for (auto& each : mc->vars_) {
    delete each.second;
  }
  Variable* result = mc->return_var_;
  context_.pop();
  delete mc;
  //��������ֵ��һ����ֵ��
  return result;
}

void Interpreter::RegisterGlobalVariable(VariableDefineStmt* v) {
  auto it = std::find_if(global_var_.begin(), global_var_.end(), [v](VariableDefineStmt* each)->bool {
    return v->var_name_ == each->var_name_;
  });
  assert(it == global_var_.end());
  global_var_.push_back(v);
}