#pragma once
#include <vector>
#include <cassert>
#include "comprehensive_type.h"

class Expression;

class Variable {
public:
  ComprehensiveType type_name_;
  std::string id_name_;
  enum class Category { Lvalue, Rvalue };
  Category cate_;

  static void HandleLife(Variable* v) {
    if (v->IsRValue()) {
      delete v;
    }
  }

  bool IsLValue() const {
    return cate_ == Category::Lvalue;
  }

  bool IsRValue() const {
    return !IsLValue();
  }

  // 通过一组表达式构造变量，在调用该函数前变量的type_name_和id_name_应该已经被设置。
  // 如果表达式为空，则调用默认构造接口DefaultConstruct.
  virtual void ConstructByExpression(const std::vector<Expression*>& constructors, Variable::Category cate) = 0;
  // 默认构造变量，在调用该函数之前变量的type_name_和id_name_应该已经被设置。
  virtual void DefaultConstruct(Variable::Category cate) = 0;
  // 通过已有变量复制一个新的变量，继承type_name_和id_name以及value，cate_由参数指定。
  virtual Variable* Copy(Variable::Category cate) = 0;
  // 修改变量的cate_为参数指定
  virtual void ChangeCategory(Variable::Category cate) = 0;
  // 赋值操作，不改变id_name_和cate_，必须在相同类型的变量间进行。
  virtual void Assign(Variable* v) = 0;
  // 根据变量名称查询数据成员，如果没找到或者不存在则返回nullptr。
  virtual Variable* FindMember(const std::string& name) = 0;
  virtual ~Variable() {

  }
};

class VoidVariable : public Variable {
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override {};
  void DefaultConstruct(Variable::Category cate) override {};
  Variable* Copy(Variable::Category cate) override { return nullptr; };
  void ChangeCategory(Variable::Category cate) override {};
  void Assign(Variable*) override {};
  Variable* FindMember(const std::string& name) override { return nullptr; };
  ~VoidVariable() {};
};

class AbstractVariable : public Variable {
public:
  std::vector<Variable*> members_;

  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~AbstractVariable();
};

class StringVariable : public Variable {
public:
  std::string val_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~StringVariable();
};

class IntVariable : public Variable {
public:
  int64_t val_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~IntVariable();
};

class BoolVariable : public Variable {
public:
  bool val_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~BoolVariable();
};

class DoubleVariable : public Variable {
public:
  double val_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~DoubleVariable();
};

class ByteVariable : public Variable {
public:
  uint8_t val_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~ByteVariable();
};

class PointerVariable : public Variable {
public:
  Variable* ptr_;
  void ConstructByExpression(const std::vector<Expression*>&, Variable::Category cate) override;
  void DefaultConstruct(Variable::Category cate) override;
  Variable* Copy(Variable::Category cate) override;
  void ChangeCategory(Variable::Category cate) override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~PointerVariable();
};

Variable* CreateVariable(const ComprehensiveType& type_name);

template<typename T>
T* VariableCast(Variable* v) {
  return static_cast<T*>(v);
}