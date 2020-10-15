#pragma once

#include <unordered_map>
#include "scanner.h"

class KeyWords {
 public:
  static KeyWords& instance();

  bool find(const std::string& id) const {
    return key_words_.find(id) != key_words_.end();
  }

  TOKEN get(const std::string& id) {
    return key_words_[id];
  }

 private:
  KeyWords();
  std::unordered_map<std::string, TOKEN> key_words_;
};