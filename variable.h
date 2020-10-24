#pragma once
#include <vector>
#include <cassert>
class Expression;

class Variable {
public:
  std::string type_name_;
  std::string id_name_;
  enum class Category { Lvalue, Rvalue };
  Category cate_;
  virtual void ConstructByExpression(const std::vector<Expression*>& constructors) = 0;
  virtual Variable* Copy() = 0;
  virtual void Assign(Variable* v) = 0;
  virtual Variable* FindMember(const std::string& name) = 0;
  virtual ~Variable() {

  }
};

class AbstractVariable : public Variable {
public:
  std::vector<Variable*> members_;

  void ConstructByExpression(const std::vector<Expression*>&) override;
  Variable* Copy() override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~AbstractVariable();
};

class StringVariable : public Variable {
public:
  std::string val_;
  void ConstructByExpression(const std::vector<Expression*>&) override;
  Variable* Copy() override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~StringVariable();
};

class IntVariable : public Variable {
public:
  int64_t val_;
  void ConstructByExpression(const std::vector<Expression*>&) override;
  Variable* Copy() override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~IntVariable();
};

class BoolVariable : public Variable {
public:
  bool val_;
  void ConstructByExpression(const std::vector<Expression*>&) override;
  Variable* Copy() override;
  void Assign(Variable*) override;
  Variable* FindMember(const std::string& name) override;
  ~BoolVariable();
};

Variable* CreateVariable(const std::string& type_name);