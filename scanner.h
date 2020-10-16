#pragma once
#include <string>
#include <vector>
#include <any>
#include <cassert>

enum class TOKEN {
  // �հ׷���
  SPACE, //�ո�
  TAB,  //tab
  ENTER, //�س�
  LINEFEED, //����

  //������������
  INT, //int
  DOUBLE, //double
  CHAR, //char
  STRING, //�ַ���
  BOOL, //bool
  VOID,

  //���̿������
  IF,
  ELSE,
  WHILE,
  FOR,
  BREAK,
  CONTINUE,
  RETURN,

  //����
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  LEFT_BRACKETS,
  RIGHT_BRACKETS,
  LEFT_BRACE,
  RIGHT_BRACE,

  //�ֺ�
  SEMICOLON,

  //ð��
  COLON,

  //С����
  DECIMAL_POINT,

  //����
  COMMA,

  //�������
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

  //����
  FUNC,
  //����
  TYPE,

  INVALID,
  //������־
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