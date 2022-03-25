#pragma once

enum class TOKEN {
  // �հ׷���
  SPACE, //�ո�
  TAB,  //tab
  ENTER, //�س�
  LINEFEED, //����

  //������������
  INT, //int
  DOUBLE, //double
  BYTE, //byte
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
  //ȡ��ַ
  ADDRESS_OF,

  ID,
  STRING_ITERAL,
  INT_ITERAL,
  DOUBLE_ITERAL,
  TRUE,
  FALSE,
  BYTE_ITERAL,

  //����
  FUNC,
  //����
  TYPE,
  //����
  METHOD,

  FN,
  LET,
  NEW,
  
  INVALID,
  //������־
  TEOF
};

const char* GetTokenStr(TOKEN token);