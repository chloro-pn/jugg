#pragma once
#include <string>
#include <vector>
#include "type.h"

class Variable {
 public:
  std::string variable_name_;
  Type* type_;
};

class AbstraceVariable : public Variable {
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