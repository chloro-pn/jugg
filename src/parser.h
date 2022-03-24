#pragma once
#include "ast_node.h"
#include "scanner.h"
#include "func.h"
#include "type.h"
#include "comprehensive_type.h"
#include <vector>
#include <cassert>
#include <stack>

size_t FindMatchedParenthesis(const std::vector<Token>& tokens, size_t begin);

size_t FindMatchedBrace(const std::vector<Token>& tokens, size_t begin);

size_t FindNextSemicolon(const std::vector<Token>& tokens, size_t begin);

size_t FindNextComma(const std::vector<Token>& tokens, size_t begin);

ComprehensiveType ParseComprehensiveType(const std::vector<Token>& tokens, size_t begin, size_t& end);

//�������ñ���ʽ�Ľ���, begin�Ǻ������֣�end�������š�
FuncCallExpr* ParseFuncCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end);

//�������ʱ���ʽ�Ľ���
MethodCallExpr* ParseMethodCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end);

// end�Ǳ���ʽ���һ��token����һ��token����ֵ��
// Ŀǰû�д�����������ʽ.
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end);

BlockStmt* ParseBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end);

FMBlockStmt* ParseFMBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end);

// ������begin����ʼ��һ����䣬���ظ�����Ӧ�Ķ���ָ�룬����end�޸�Ϊ��������һ��token����һ��token����ֵ��
Statement* ParseStatement(const std::vector<Token>& tokens, size_t begin, size_t& end);

std::vector<std::pair<std::string, ComprehensiveType>> ParseParameterList(const std::vector<Token>& tokens, size_t begin, size_t end);

Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end);

Method ParseMethod(const std::vector<Token>& tokens, size_t begin, size_t& end);

Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end);

VariableDefineStmt* ParseVariableDefinition(const std::vector<Token>& token, size_t begin, size_t& end);

// �����Ķ����ṹӦ����type�Ķ��塢func�Ķ��壬ȫ�ֱ����Ķ���
void Parse(const std::vector<Token>& tokens);