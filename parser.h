#pragma once
#include "ast_node.h"
#include "scanner.h"
#include "operator.h"
#include "func.h"
#include "type.h"
#include "variable.h"
#include <vector>
#include <cassert>
#include <stack>

size_t FindMatchedParenthesis(const std::vector<Token>& tokens, size_t begin);

size_t FindMatchedBrace(const std::vector<Token>& tokens, size_t begin);

size_t FindNextSemicolon(const std::vector<Token>& tokens, size_t begin);

// end是表达式最后一个token的下一个token索引值。
// 目前没有处理函数调用表达式和索引表达式.
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end);

BlockStmt* ParseBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end);

// 解析从begin处开始的一条语句，返回该语句对应的对象指针，并将end修改为该语句最后一个token的下一个token索引值。
Statement* ParseStatement(const std::vector<Token>& tokens, size_t begin, size_t& end);

std::vector<std::pair<std::string, std::string>> ParseParameterList(const std::vector<Token>& tokens, size_t begin, size_t end);

Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end);

Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end);

Variable ParseVariableDefinition(const std::vector<Token>& token, size_t begin, size_t& end);

//TODO
//void ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end);

// 解析的顶级结构应该是type的定义、func的定义，全局变量的定义
// 目前考虑func的定义
void Parse(const std::vector<Token>& tokens);