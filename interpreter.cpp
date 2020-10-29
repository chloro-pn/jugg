#include "interpreter.h"
#include "type.h"
#include "ast_node.h"
#include "variable.h"
#include "scanner.h"
#include "parser.h"
#include <algorithm>
#include <iostream>

Interpreter& Interpreter::instance() {
  static Interpreter obj;
  return obj;
}

static void inner_print(inner_func_context* fc) {
  fc->return_var_ = new VoidVariable;
  fc->return_var_->id_name_ = "void";
  for (auto& each : fc->vars_) {
    if (each->type_name_ == "int") {
      std::cout << static_cast<IntVariable*>(each)->val_;
    }
    else if (each->type_name_ == "string") {
      std::cout << static_cast<StringVariable*>(each)->val_;
    }
    else if (each->type_name_ == "double") {
      std::cout << static_cast<DoubleVariable*>(each)->val_;
    }
    else if (each->type_name_ == "bool") {
      std::cout << static_cast<BoolVariable*>(each)->val_;
    }
    else if (each->type_name_ == "byte") {
      std::cout << static_cast<ByteVariable*>(each)->val_;
    }
    else {
      assert(false);
    }
  }
}

static void inner_len(inner_func_context* fc) {
  assert(fc->vars_.size() == 1 && fc->vars_[0]->type_name_ == "string");
  size_t i = static_cast<StringVariable*>(fc->vars_[0])->val_.size();
  IntVariable* v = new IntVariable;
  v->cate_ = Variable::Category::Rvalue;
  v->id_name_ = "tmp";
  v->type_name_ = "int";
  v->val_ = i;
  fc->return_var_ = v;
}

//��ʼ�������ȫ��������
Interpreter::Interpreter() {
  context_.push(new BlockContext);
  inner_func_["print"] = inner_print;
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
    inner_func_context* ifc = new inner_func_context;
    ifc->func_name = func_name;
    //main����û�в�������
    ifc->vars_ = variables;
    inner_func_[func_name](ifc);
    for (auto& each : ifc->vars_) {
      delete each;
    }
    Variable* result = ifc->return_var_;
    delete ifc;
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
    variables[i]->id_name_ = func.parameter_type_list_[i].first;
    fc->vars_[func.parameter_type_list_[i].first] = variables[i];
  }

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
    variables[i]->id_name_ = method.parameter_type_list_[i].first;
    mc->vars_[method.parameter_type_list_[i].first] = variables[i];
  }
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