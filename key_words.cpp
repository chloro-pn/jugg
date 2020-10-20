#include "key_words.h"

static void register_buildin_keyword(std::unordered_map<std::string, TOKEN>& container) {
  container["if"] = TOKEN::IF;
  container["else"] = TOKEN::ELSE;
  container["while"] = TOKEN::WHILE;
  container["for"] = TOKEN::FOR;
  container["continue"] = TOKEN::CONTINUE;
  container["break"] = TOKEN::BREAK;
  container["return"] = TOKEN::RETURN;
  container["true"] = TOKEN::TRUE;
  container["false"] = TOKEN::FALSE;
  container["string"] = TOKEN::STRING;
  container["int"] = TOKEN::INT;
  container["double"] = TOKEN::DOUBLE;
  container["bool"] = TOKEN::BOOL;
  container["char"] = TOKEN::CHAR;
  container["void"] = TOKEN::VOID;
  container["func"] = TOKEN::FUNC;
  container["type"] = TOKEN::TYPE;
  container["method"] = TOKEN::METHOD;
}

KeyWords& KeyWords::instance() {
  static KeyWords obj;
  return obj;
}

KeyWords::KeyWords() {
  register_buildin_keyword(key_words_);
}