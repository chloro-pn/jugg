#pragma once
#include <string>
#include <vector>

class ComprehensiveType {
public:
  std::string base_type_;

  enum class TYPE {SCALE, ARRAY};
  TYPE type_;
  size_t array_num_;

  ComprehensiveType() = default;
  ComprehensiveType(const ComprehensiveType&) = default;
  ComprehensiveType& operator=(const ComprehensiveType& other) = default;
  bool operator==(const ComprehensiveType& other) const {
    if (base_type_ != other.base_type_) {
      return false;
    }
    return true;
  }

  bool IsBaseType() const {
    return true;
  }

  bool IsBaseType(const std::string& type_name) const {
    return base_type_ == type_name;
  }
};