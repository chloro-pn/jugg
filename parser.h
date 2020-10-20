#pragma once
#include "ast_node.h"
#include "scanner.h"
#include "operator.h"
#include "func.h"
#include "type.h"
#include <vector>
#include <cassert>
#include <stack>

size_t FindMatchedParenthesis(const std::vector<Token>& tokens, size_t begin);

size_t FindMatchedBrace(const std::vector<Token>& tokens, size_t begin);

size_t FindNextSemicolon(const std::vector<Token>& tokens, size_t begin);

size_t FindNextComma(const std::vector<Token>& tokens, size_t begin);

//�������ñ��ʽ�Ľ���, begin�Ǻ������֣�end�������š�
FuncCallExpr* ParseFuncCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end);

// end�Ǳ��ʽ���һ��token����һ��token����ֵ��
// Ŀǰû�д����������ʽ.
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end);

BlockStmt* ParseBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end);

// ������begin����ʼ��һ����䣬���ظ�����Ӧ�Ķ���ָ�룬����end�޸�Ϊ��������һ��token����һ��token����ֵ��
Statement* ParseStatement(const std::vector<Token>& tokens, size_t begin, size_t& end);

std::unordered_map<std::string, std::string> ParseParameterList(const std::vector<Token>& tokens, size_t begin, size_t end);

Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end);

Method ParseMethod(const std::vector<Token>& tokens, size_t begin, size_t& end);

Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end);

VariableDefineStmt* ParseVariableDefinition(const std::vector<Token>& token, size_t begin, size_t& end);

// �����Ķ����ṹӦ����type�Ķ��塢func�Ķ��壬ȫ�ֱ����Ķ���
void Parse(const std::vector<Token>& tokens);