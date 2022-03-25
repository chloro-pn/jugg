#pragma once
#include <vector>
#include <cassert>
#include "comprehensive_type.h"

class Expression;

class Variable {
public:
  // 每个变量都具有一个类型
  ComprehensiveType type_name_;
  // 变量名字
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

  // 通过一组表达式构建变量，在调用本函数前type_name_和id_name_应该已经被设置
  // 如果表达式为空，则调用默认构造接口DefaultConstruct
  virtual void ConstructByExpression(const std::vector<Expression*>& constructors, Variable::Category cate) = 0;
  // 默认构造变量，调用本函数前type_name_和id_name_应该已经被设置
  virtual void DefaultConstruct(Variable::Category cate) = 0;
  // 变量复制操作，继承type_name和id_name，cate由参数指定
  virtual Variable* Copy(Variable::Category cate) = 0;
  // 修改category
  virtual void ChangeCategory(Variable::Category cate) = 0;
  // 赋值操作
  virtual void Assign(Variable* v) = 0;
  // 根据名称查找数据成员
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