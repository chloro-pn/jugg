#pragma once
#include <vector>

class Variable {
public:
  std::string type_name_;
  std::string id_name_;
};

class AbstractVariable : public Variable {
public:
  std::vector<Variable*> members_;
};

class StringVariable : public Variable {
public:
  std::string val_;
};

class IntVariable : public Variable {
public:
  int64_t val_;
};