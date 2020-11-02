#include "scanner.h"
#include "key_words.h"
#include <regex>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

template<typename T>
Token CreateToken(TOKEN token, T t);

template<>
Token CreateToken(TOKEN token, std::string str);

template<>
Token CreateToken(TOKEN token, int64_t i);

template<>
Token CreateToken(TOKEN token, double d);

template<>
Token CreateToken(TOKEN token, uint8_t c);

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
Token CreateToken(TOKEN token, uint8_t c) {
  Token result;
  result.token = token;
  result.type = Token::TYPE::BYTE;
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

static uint8_t string_to_uint8_t(const std::string& str) {
  assert(str.size() == 4 && str[0] == '0' && str[1] == 'X');
  int b1 = 0, b2 = 0;
  if (str[2] >= '0' && str[2] <= '9') {
    b1 = str[2] - '0';
  }
  else {
    b1 = str[2] - 'A' + 10;
  }
  if (str[3] >= '0' && str[3] <= '9') {
    b2 = str[3] - '0';
  }
  else {
    b2 = str[3] - 'A' + 10;
  }
  return static_cast<uint8_t>(b1 * 16 + b2);
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
    else if (std::regex_search(begin, end, result, std::regex("&"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::ADDRESS_OF));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("\\|\\|"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::OR));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("!"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::NOT));
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
    else if (std::regex_search(begin, end, result, std::regex("\"(.*?)\""), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::STRING_ITERAL, result[1].str()));
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
    else if (std::regex_search(begin, end, result, std::regex("0X[0-9A-F][0-9A-F]"), std::regex_constants::match_continuous)) {
      tokens.push_back(CreateToken(TOKEN::BYTE_ITERAL, string_to_uint8_t(result[0].str())));
      begin = result[0].second;
    }
    else if (std::regex_search(begin, end, result, std::regex("[-+]?(0|([1-9][0-9]*))"), std::regex_constants::match_continuous)) {
      int64_t num;
      std::stringstream stream;
      stream << result[0].str();
      stream >> num;
      tokens.push_back(CreateToken(TOKEN::INT_ITERAL, num));
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

Scanner& Scanner::instance() {
  static Scanner obj;
  return obj;
}

const std::vector<Token>& Scanner::Scan(const std::string& filename) {
  std::ifstream in(filename.c_str());
  std::stringstream buffer;
  buffer << in.rdbuf();
  std::string str(buffer.str());
  tokens_ = scan(str);
  tokens_.push_back(CreateToken(TOKEN::TEOF));
  return tokens_;
}

Scanner::Scanner() {

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
      else if (it->type == Token::TYPE::BYTE) {
        std::cout << "char : " << std::any_cast<uint8_t>(it->attr);
      }
    }
    std::cout << std::endl;
    ++index;
  }
  std::cout << "TEOF" << std::endl;
}