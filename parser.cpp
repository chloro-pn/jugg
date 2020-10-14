#include "parser.h"

#define FindMatch(FuncName, x, y) \
size_t FuncName(const std::vector<Token>& tokens, size_t begin) { \
  assert(tokens[begin].token == (x)); \
  size_t counter = 0; \
  size_t index = begin; \
  while (index < tokens.size()) { \
    if (tokens[index].token == (x)) { \
      ++counter; \
    } \
    else if (tokens[index].token == (y)) { \
      --counter; \
      if (counter == 0) { \
        return index; \
      } \
    } \
    ++index; \
  } \
  return -1; \
}

FindMatch(FindMatchedParenthesis, TOKEN::LEFT_PARENTHESIS, TOKEN::RIGHT_PARENTHESIS);

FindMatch(FindMatchedBrace, TOKEN::LEFT_BRACE, TOKEN::RIGHT_BRACE);

//因为要处理空语句，因此begin本身可能就是分号。
size_t FindNextSemicolon(const std::vector<Token>& tokens, size_t begin) {
  size_t index = begin;
  while (index < tokens.size()) {
    if (tokens[index].token == TOKEN::SEMICOLON) {
      return index;
    }
    ++index;
  }
  return -1;
}

//level值越大，优先级越低
static int CompareOperatorLevel(TOKEN t1, TOKEN t2) {
  if (OperatorSet::instance().GetLevel(t1) > OperatorSet::instance().GetLevel(t2)) {
    return -1;
  }
  else if (OperatorSet::instance().GetLevel(t1) < OperatorSet::instance().GetLevel(t2)) {
    return 1;
  }
  return 0;
}

// end是表达式最后一个token的下一个token索引值。
// TODO : 处理函数调用表达式和索引表达式, 变量定义表达式
// 解析函数定义文法
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end) {
  if (begin == end) {
    return nullptr; // 可能是空语句
  }
  std::stack<Expression*> operands;
  std::stack<TOKEN> operators;
  for (size_t i = begin; i < end; ++i) {
    TOKEN current_token = tokens[i].token;
    //左括号特殊处理，因为其改变了运算优先级
    if (current_token == TOKEN::LEFT_PARENTHESIS) {
      size_t match_parent = FindMatchedParenthesis(tokens, i);
      //递归求解括号中的表达式
      operands.push(ParseExpression(tokens, i, match_parent));
      i = match_parent;
      continue;
    }
    //所有的右括号TOKEN应该被跳过
    assert(current_token != TOKEN::RIGHT_PARENTHESIS);
    if (OperatorSet::instance().find(current_token) == false) {
      //是操作数，直接入栈
      IdExpr* idexpr = new IdExpr;
      idexpr->token_index_ = i;
      operands.push(idexpr);
      continue;
    }
    else {
      while (operators.empty() == false && CompareOperatorLevel(current_token, operators.top()) <= 0) {
        //运算, operands中至少有两个表达式
        BinaryExpr* binexpr = new BinaryExpr;
        //operators一定不为空
        binexpr->operator_token_ = operators.top();
        operators.pop();
        binexpr->right_ = operands.top();
        operands.pop();
        binexpr->left_ = operands.top();
        operands.pop();
        operands.push(binexpr);
      }
      operators.push(current_token);
    }
  }
  //处理剩余的运算符
  while (operators.empty() == false) {
    BinaryExpr* binexpr = new BinaryExpr;
    binexpr->operator_token_ = operators.top();
    operators.pop();
    binexpr->right_ = operands.top();
    operands.pop();
    binexpr->left_ = operands.top();
    operands.pop();
    operands.push(binexpr);
  }
  //最后应该只剩下一个总表达式.
  assert(operands.size() == 1);
  return operands.top();
}

BlockStmt* ParseBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end) {
  BlockStmt* result = new BlockStmt();
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  assert(tokens[end].token == TOKEN::RIGHT_BRACE);
  ++begin;
  while (true) {
    if (begin == end) {
      break;
    }
    size_t next = 0;
    result->block_.push_back(ParseStatement(tokens, begin, next));
    begin = next;
  }
  return result;
}

//解析从begin处开始的一条语句，返回该语句对应的对象指针，并将end修改为该语句最后一个token的下一个token索引值。
Statement* ParseStatement(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Statement* result = nullptr;
  if (tokens[begin].token == TOKEN::IF) {
    IfStmt* tmp = new IfStmt();
    ++begin;
    assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
    size_t match_parent = FindMatchedParenthesis(tokens, begin);
    ++begin;
    tmp->check_ = ParseExpression(tokens, begin, match_parent);

    begin = match_parent + 1;
    assert(tokens[begin].token == TOKEN::LEFT_BRACE);
    size_t match_brace = FindMatchedBrace(tokens, begin);
    tmp->if_block_ = ParseBlockStmt(tokens, begin, match_brace);

    begin = match_brace + 1;
    if (tokens[begin].token == TOKEN::ELSE) {
      ++begin;
      assert(tokens[begin].token == TOKEN::LEFT_BRACE);
      end = FindMatchedBrace(tokens, begin);
      tmp->else_block_ = ParseBlockStmt(tokens, begin, end);
      ++end;
    }
    else {
      end = match_brace + 1;
      tmp->else_block_ = nullptr;
    }
    result = tmp;
    return result;
  }
  else if (tokens[begin].token == TOKEN::WHILE) {
    WhileStmt* tmp = new WhileStmt();
    ++begin;
    size_t match_parent = FindMatchedParenthesis(tokens, begin);
    ++begin;
    tmp->check_ = ParseExpression(tokens, begin, match_parent);

    begin = match_parent + 1;
    assert(tokens[begin].token == TOKEN::LEFT_BRACE);
    size_t match_brace = FindMatchedBrace(tokens, begin);
    tmp->block_ = ParseBlockStmt(tokens, begin, match_brace);
    result = tmp;
    end = match_brace + 1;
    return result;
  }
  else if (tokens[begin].token == TOKEN::FOR) {
    ForStmt* tmp = new ForStmt();
    ++begin;
    assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
    size_t match_parent = FindMatchedParenthesis(tokens, begin);
    ++begin;
    size_t next_semicolon = FindNextSemicolon(tokens, begin);
    tmp->init_ = ParseExpression(tokens, begin, next_semicolon);

    begin = next_semicolon + 1;
    next_semicolon = FindNextSemicolon(tokens, begin);
    tmp->check_ = ParseExpression(tokens, begin, next_semicolon);

    begin = next_semicolon + 1;
    tmp->update_ = ParseExpression(tokens, begin, match_parent);

    begin = match_parent + 1;
    assert(tokens[begin].token == TOKEN::LEFT_BRACE);
    size_t match_brace = FindMatchedBrace(tokens, begin);
    tmp->block_ = ParseBlockStmt(tokens, begin, match_brace);
    result = tmp;
    end = match_brace + 1;
    return result;
  }
  else if (tokens[begin].token == TOKEN::RETURN) {
    end = begin + 1;
    return new ReturnStmt;
  }
  else if (tokens[begin].token == TOKEN::CONTINUE) {
    end = begin + 1;
    return new ContinueStmt;
  }
  else if (tokens[begin].token == TOKEN::BREAK) {
    end = begin + 1;
    return new BreakStmt;
  }
  else {
    // 表达式语句.
    ExpressionStmt* tmp = new ExpressionStmt();
    end = FindNextSemicolon(tokens, begin);
    tmp->root_ = ParseExpression(tokens, begin, end);
    ++end;
    result = tmp;
    return result;
  }
}