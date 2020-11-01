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
  virtual void ConstructByExpression(const std::vector<Expression*>& constructors, Variable::Category cate) = 0;
  virtual void DefaultConstruct(Variable::Category cate) = 0;
  virtual Variable* Copy(Variable::Category cate) = 0;
  virtual void ChangeCategory(Variable::Category cate) = 0;
  virtual void Assign(Variable* v) = 0;
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

Variable* CreateVariable(const std::string& type_name);