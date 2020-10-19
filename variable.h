#pragma once
#include <string>
#include <vector>

class Variable {
 public:
  std::string variable_name_;
  std::string type_name_;

  virtual ~Variable() {

  }
};

class AbstractVariable : public Variable {
 public:
  std::vector<Variable*> data_members_;
};

class StringVariable : public Variable {
 public:
  std::string var_;
};

class IntVariable : public Variable {
 public:
   int64_t var_;
};

Variable* CreateVariable(const std::string& type);