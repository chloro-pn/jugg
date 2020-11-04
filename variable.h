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

  // ͨ��һ����ʽ����������ڵ��øú���ǰ������type_name_��id_name_Ӧ���Ѿ������á�
  // ������ʽΪ�գ������Ĭ�Ϲ���ӿ�DefaultConstruct.
  virtual void ConstructByExpression(const std::vector<Expression*>& constructors, Variable::Category cate) = 0;
  // Ĭ�Ϲ���������ڵ��øú���֮ǰ������type_name_��id_name_Ӧ���Ѿ������á�
  virtual void DefaultConstruct(Variable::Category cate) = 0;
  // ͨ�����б�������һ���µı������̳�type_name_��id_name�Լ�value��cate_�ɲ���ָ����
  virtual Variable* Copy(Variable::Category cate) = 0;
  // �޸ı�����cate_Ϊ����ָ��
  virtual void ChangeCategory(Variable::Category cate) = 0;
  // ��ֵ���������ı�id_name_��cate_����������ͬ���͵ı�������С�
  virtual void Assign(Variable* v) = 0;
  // ���ݱ������Ʋ�ѯ���ݳ�Ա�����û�ҵ����߲������򷵻�nullptr��
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