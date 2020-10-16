#pragma once
#include <string>
#include <vector>
#include <any>
#include <cassert>

enum class TOKEN {
  // 空白符号
  SPACE, //空格
  TAB,  //tab
  ENTER, //回车
  LINEFEED, //换行

  //基本数据类型
  INT, //int
  DOUBLE, //double
  CHAR, //char
  STRING, //字符串
  BOOL, //bool
  VOID,

  //流程控制语句
  IF,
  ELSE,
  WHILE,
  FOR,
  BREAK,
  CONTINUE,
  RETURN,

  //括号
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  LEFT_BRACKETS,
  RIGHT_BRACKETS,
  LEFT_BRACE,
  RIGHT_BRACE,

  //分号
  SEMICOLON,

  //冒号
  COLON,

  //小数点
  DECIMAL_POINT,

  //逗号
  COMMA,

  //运算符号
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  AND,
  OR,
  NOT,
  ASSIGN,
  COMPARE,
  GREATER_THAN,
  LESS_THAN,

  ID,
  STRING_ITERAL,
  INT_ITERAL,
  DOUBLE_ITERAL,
  TRUE,
  FALSE,
  CHAR_ITERAL,

  //函数
  FUNC,
  //类型
  TYPE,

  INVALID,
  //结束标志
  TEOF
};

const char* GetTokenStr(TOKEN token);

struct Token {
  TOKEN token;
  enum class TYPE {STRING, INT, DOUBLE, CHAR, BOOL, NONE};
  TYPE type;
  std::any attr;

  template<typename T>
  T get() const {
    return std::any_cast<T>(attr);
  }

  Token() = default;

  Token(const Token& other) = default;
};

std::vector<Token> scan(std::string file);

void TokenPrint(const std::vector<Token>& tokens);

class Scanner {
 public:
  explicit Scanner(std::string filename);

  const std::vector<Token>& tokens() const {
    return tokens_;
  }

 private:
  std::vector<Token> tokens_;
};