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

  //���̿������
  IF,
  ELSE,
  WHILE,
  FOR,
  BREAK,
  CONTINUE,

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

  //������־
  TEOF
};

const char* GetTokenStr(TOKEN token);

struct Token {
  TOKEN token;
  enum class TYPE {STRING, INT, DOUBLE, CHAR, BOOL, NONE};
  TYPE type;
  std::any attr;

  Token() = default;

  Token(const Token& other) = default;
};

std::vector<Token> scan(std::string file);

void TokenPrint(const std::vector<Token>& tokens);

class Scanner {
 public:
  explicit Scanner(std::string filename);

  Token getNextToken();

 private:
  std::vector<Token> tokens_;
  int64_t current_index_;
};