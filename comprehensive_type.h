#pragma once
#include <string>
#include <vector>

class ComprehensiveType {
public:
  std::string base_type_;
  enum Modifier { Pointer, Reference, Array };
  std::vector<Modifier> modifiers_;

  ComprehensiveType() = default;
  ComprehensiveType(const ComprehensiveType&) = default;
  ComprehensiveType& operator=(const ComprehensiveType& other) = default;
  bool operator==(const ComprehensiveType& other) const {
    if (base_type_ != other.base_type_) {
      return false;
    }
    if (modifiers_.size() != other.modifiers_.size()) {
      return false;
    }
    for (size_t i = 0; i < modifiers_.size(); ++i) {
      if (modifiers_[i] != other.modifiers_[i]) {
        return false;
      }
    }
    return true;
  }

  bool IsBaseType() const {
    return modifiers_.empty() == true;
  }

  bool IsBaseType(const std::string& type_name) const {
    return modifiers_.empty() == true && base_type_ == type_name;
  }

  bool PtrType() const {
    return !IsBaseType() && modifiers_.back() == Modifier::Pointer;
  }
};