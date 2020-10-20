#include "scanner.h"
#include "key_words.h"
#include <regex>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

const char* GetTokenStr(TOKEN token) {
  switch (token) {
  case TOKEN::SPACE:
    return "space";
  case TOKEN::TAB:
    return "tab";
  case TOKEN::ENTER:
    return "enter";
  case TOKEN::LINEFEED:
    return "linefeed";
  case TOKEN::INT:
    return "int";
  case TOKEN::DOUBLE:
    return "double";
  case TOKEN::STRING:
    return "string";
  case TOKEN::BOOL:
    return "bool";
  case TOKEN::CHAR:
    return "char";
  case TOKEN::VOID:
    return "void";
  case TOKEN::IF:
    return "if";
  case TOKEN::WHILE:
    return "while";
  case TOKEN::ELSE:
    return "else";
  case TOKEN::BREAK:
    return "break";
  case TOKEN::CONTINUE:
    return "continue";
  case TOKEN::RETURN:
    return "return";
  case TOKEN::LEFT_PARENTHESIS:
    return "(";
  case TOKEN::RIGHT_PARENTHESIS:
    return ")";
  case TOKEN::LEFT_BRACKETS:
    return "[";
  case TOKEN::RIGHT_BRACKETS:
    return "]";
  case TOKEN::LEFT_BRACE:
    return "{";
  case TOKEN::RIGHT_BRACE:
    return "}";
  case TOKEN::SEMICOLON:
    return ";";
  case TOKEN::COLON:
    return ":";
  case TOKEN::DECIMAL_POINT:
    return ".";
  case TOKEN::COMMA:
    return ",";
  case TOKEN::PLUS:
    return "+";
  case TOKEN::MINUS:
    return "-";
  case TOKEN::MULTIPLY:
    return "*";
  case TOKEN::DIVIDE:
    return "/";
  case TOKEN::AND:
    return "&";
  case TOKEN::OR:
    return "|";
  case TOKEN::NOT:
    return "~";
  case TOKEN::ASSIGN:
    return "=";
  case TOKEN::COMPARE:
    return "==";
  case TOKEN::GREATER_THAN:
    return ">";
  case TOKEN::LESS_THAN:
    return "<";
  case TOKEN::STRING_ITERAL:
    return "string_iteral";
  case TOKEN::TRUE:
    return "true";
  case TOKEN::FALSE:
    return "false";
  case TOKEN::INT_ITERAL:
    return "int_iteral";
  case TOKEN::DOUBLE_ITERAL:
    return "double_iteral";
  case TOKEN::CHAR_ITERAL:
    return "char_iteral";
  case TOKEN::FUNC:
    return "func";
  case TOKEN::TYPE:
    return "type";
  case TOKEN::METHOD:
    return "method";
  case TOKEN::ID:
    return "id";
  case TOKEN::TEOF:
    return "teof";
  default:
    assert(token == TOKEN::INVALID);
    return "invalid";
  }
}

template<typename T>
Token CreateToken(TOKEN token, T t);

template<>
Token CreateToken(TOKEN token, std::string str);

template<>
Token CreateToken(TOKEN token, int64_t i);

template<>
Token CreateToken(TOKEN token, double d);

template<>
Token CreateToken(TOKEN token, char c);

template<>
Token CreateToken(TOKEN token, bool b);

Token CreateToken(TOKEN token);

template<>
Token CreateToken(TOKEN token, std::string str) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::STRING;
  result.attr = str;
  return result;
}

template<>
Token CreateToken(TOKEN token, int64_t i) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::INT;
  result.attr = i;
  return result;
}

template<>
Token CreateToken(TOKEN token, double d) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::DOUBLE;
  result.attr = d;
  return result;
}

template<>
Token CreateToken(TOKEN token, char c) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::CHAR;
  result.attr = c;
  return result;
}

template<>
Token CreateToken(TOKEN token, bool b) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::BOOL;
  result.attr = b;
  return result;
}

Token CreateToken(TOKEN token) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::NONE;
  return result;
}

std::vector<Token> scan(std::string file) {
  std::vector<Token> tokens;
  auto begin = file.cbegin();
  auto end = file.cend();
  while (true) {
    std::smatch result;
    if (std::regex_search(begin, end, result, std::regex("[ \\r\\n\\t]"), std::regex_constants::match_continuous)) {
      ++begin;
    }
    else if (std::regex_search(begin, end, result, std::regex("//.*?\\n"), std::regex_constants::match_continuous)) {
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\("), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::LEFT_PARENTHESIS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\)"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::RIGHT_PARENTHESIS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\["), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::LEFT_BRACKETS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\]"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::RIGHT_BRACKETS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\{"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::LEFT_BRACE));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\}"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::RIGHT_BRACE));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex(";"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::SEMICOLON));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex(":"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::COLON));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\."), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::DECIMAL_POINT));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex(","), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::COMMA));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\+"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::PLUS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("-"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::MINUS));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\*"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::MULTIPLY));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("/"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::DIVIDE));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("&&"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::AND));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\|\\|"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::OR));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("!"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::IF));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("=="), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::COMPARE));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("="), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::ASSIGN));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex(">"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::GREATER_THAN));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("<"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::LESS_THAN));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\".*?\""), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::STRING_ITERAL, result[0].str()));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("[-+]?(0|[1-9][0-9]*)\\.[0-9]*"), std::regex_constants::match_continuous)) {
      double num;
      std::stringstream stream;
      stream << result[0].str();
      stream >> num;
      tokens.push_back(CreateToken(TOKEN::DOUBLE_ITERAL, num));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("[-+]?(0|[1-9][0-9]*)"), std::regex_constants::match_continuous)) {
      int64_t num;
      std::stringstream stream;
      stream << result[0].str();
      stream >> num;
      tokens.push_back(CreateToken(TOKEN::INT_ITERAL, num));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\'.\'"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::CHAR_ITERAL, result[0].str()[0]));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("[a-zA-Z_][a-zA-Z0-9_]*"), std::regex_constants::match_continuous)) {
      if (KeyWords::instance().find(result[0]) == true) {
        tokens.push_back(CreateToken(KeyWords::instance().get(result[0].str()), result[0].str()));
      }
      else {
        tokens.push_back(CreateToken(TOKEN::ID, result[0].str()));
      }
      begin = result[0].second;
    }
    else {
      exit(EXIT_FAILURE);
    }
    if (begin == end) {
      break;
    }
  }
  return tokens;
}

// Scanner负责在TOKEN流最后补充teof的token。
Scanner::Scanner(std::string filename) {
  std::ifstream in(filename.c_str());
  std::stringstream buffer;
  buffer << in.rdbuf();
  std::string str(buffer.str());
  tokens_ = scan(str);
  tokens_.push_back(CreateToken(TOKEN::TEOF));
}

void TokenPrint(const std::vector<Token>& tokens) {
  size_t index = 0;
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    std::cout << index << " : " << GetTokenStr(it->token);
    if (it->type != Token::TYPE::NONE) {
      std::cout << " -> ";
      if (it->type == Token::TYPE::STRING) {
        std::cout << "string : " << std::any_cast<std::string>(it->attr);
      }
      else if (it->type == Token::TYPE::INT) {
        std::cout << "int : " << std::any_cast<int64_t>(it->attr);
      }
      else if (it->type == Token::TYPE::DOUBLE) {
        std::cout << "double : " << std::any_cast<double>(it->attr);
      }
      else if (it->type == Token::TYPE::BOOL) {
        std::cout << "bool : " << std::any_cast<bool>(it->attr);
      }
      else if (it->type == Token::TYPE::CHAR) {
        std::cout << "char : " << std::any_cast<char>(it->attr);
      }
    }
    std::cout << std::endl;
    ++index;
  }
  std::cout << "TEOF" << std::endl;
}