#include "variable.h"

Variable* CreateVariable(const std::string& type) {
  Variable* result;
  if (type == "string") {
    result = new StringVariable;
  }
  else if (type == "int") {
    result = new IntVariable;
  }
  //...
  else {
    result = new AbstractVariable;
  }
  result->type_name_ = type;
  return result;
}