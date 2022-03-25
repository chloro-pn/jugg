#pragma once

#include <unordered_map>
#include <string>

#include "token.h"

class KeyWords {
 public:
  static KeyWords& instance();

  bool Find(const std::string& id) const {
    return key_words_.find(id) != key_words_.end();
  }

  TOKEN Get(const std::string& id) {
    return key_words_[id];
  }

 private:
  KeyWords();
  std::unordered_map<std::string, TOKEN> key_words_;
};