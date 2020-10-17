#include "parser.h"
#include "func.h"
#include "scope.h"
#include <cassert>

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

//��ΪҪ�������䣬���begin������ܾ��Ƿֺš�
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

size_t FindNextCommaOrRightParenthesis(const std::vector<Token>& tokens, size_t begin) {
  size_t index = begin;
  while (index < tokens.size()) {
    if (tokens[index].token == TOKEN::COMMA || tokens[index].token == TOKEN::RIGHT_PARENTHESIS) {
      return index;
    }
    ++index;
  }
  return -1;
}

//levelֵԽ�����ȼ�Խ��
static int CompareOperatorLevel(TOKEN t1, TOKEN t2) {
  if (OperatorSet::instance().GetLevel(t1) > OperatorSet::instance().GetLevel(t2)) {
    return -1;
  }
  else if (OperatorSet::instance().GetLevel(t1) < OperatorSet::instance().GetLevel(t2)) {
    return 1;
  }
  return 0;
}

FuncCallExpr* ParseFuncCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end) {
  FuncCallExpr* result = new FuncCallExpr;
  assert(tokens[begin].token == TOKEN::ID);
  assert(tokens[begin].type == Token::TYPE::STRING);
  result->func_name_ = tokens[begin].get<std::string>();
  //ȷ����ǰ�������Ա��ҵ������Ѿ���ǰ����������б����塣
  assert(FuncSet::instance().Find(result->func_name_) == true);
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  while (true) {
    if (tokens[begin].token == TOKEN::RIGHT_PARENTHESIS) {
      break;
    }
    ++begin;
    size_t next = FindNextCommaOrRightParenthesis(tokens, begin);
    // func_name(xxxxx, xxxxx, xxxx)
    //           |    |
    //         begin end
    result->parameters_.push_back(ParseExpression(tokens, begin, next));
    begin = next;
  }
  return result;
}

// end�Ǳ��ʽ���һ��token����һ��token����ֵ��
// TODO : ���������ñ��ʽ���±���ʱ��ʽ����Ա���ʱ��ʽ��
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end) {
  if (begin == end) {
    return nullptr; // �����ǿ����
  }
  std::stack<Expression*> operands;
  std::stack<TOKEN> operators;
  for (size_t i = begin; i < end; ++i) {
    TOKEN current_token = tokens[i].token;
    //���������⴦����Ϊ��ı����������ȼ�
    if (current_token == TOKEN::LEFT_PARENTHESIS) {
      size_t match_parent = FindMatchedParenthesis(tokens, i);
      //��������Ų��Ǳ��ʽ�ĵ�һ��TOKEN������ǰ���TOKEN�������������Ӧ���Ǻ������ñ��ʽ��
      if (i > begin && OperatorSet::instance().find(tokens[i - 1].token) == false) {
        assert(operands.empty() == false);
        Expression* func_name = operands.top();
        operands.pop();
        //������Ҫȷ��func_name���ʽ��IdExpr��
        assert(tokens[i - 1].type == Token::TYPE::STRING);
        assert(tokens[i - 1].token == TOKEN::ID);
        operands.push(ParseFuncCallExpr(tokens, i - 1, match_parent));
      }
      else {
        //�ݹ���������еı��ʽ
        operands.push(ParseExpression(tokens, i, match_parent));
      }
      i = match_parent;
      continue;
    }
    //���е�������TOKENӦ�ñ�����
    assert(current_token != TOKEN::RIGHT_PARENTHESIS);
    if (OperatorSet::instance().find(current_token) == false) {
      //�ǲ�������ֱ����ջ
      IdExpr* idexpr = new IdExpr;
      idexpr->token_index_ = i;
      operands.push(idexpr);
      continue;
    }
    else {
      while (operators.empty() == false && CompareOperatorLevel(current_token, operators.top()) <= 0) {
        //����, operands���������������ʽ
        BinaryExpr* binexpr = new BinaryExpr;
        //operatorsһ����Ϊ��
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
  //����ʣ��������
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
  //���Ӧ��ֻʣ��һ���ܱ��ʽ.
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

//������begin����ʼ��һ����䣬���ظ�����Ӧ�Ķ���ָ�룬����end�޸�Ϊ��������һ��token����һ��token����ֵ��
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

    Scopes::instance().EnterNewScope(Scope::TYPE::BLOCK);
    tmp->if_block_ = ParseBlockStmt(tokens, begin, match_brace);
    Scopes::instance().LeaveScope();

    begin = match_brace + 1;
    if (tokens[begin].token == TOKEN::ELSE) {
      ++begin;
      assert(tokens[begin].token == TOKEN::LEFT_BRACE);
      end = FindMatchedBrace(tokens, begin);

      Scopes::instance().EnterNewScope(Scope::TYPE::BLOCK);
      tmp->else_block_ = ParseBlockStmt(tokens, begin, end);
      Scopes::instance().LeaveScope();

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

    Scopes::instance().EnterNewScope(Scope::TYPE::BLOCK);
    tmp->block_ = ParseBlockStmt(tokens, begin, match_brace);
    Scopes::instance().LeaveScope();

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

    Scopes::instance().EnterNewScope(Scope::TYPE::BLOCK);
    tmp->block_ = ParseBlockStmt(tokens, begin, match_brace);
    Scopes::instance().LeaveScope();

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
  else if (tokens[begin].token == TOKEN::ID && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true) {
    //�����������
    //Variable v = ParseVariableDefinition(tokens, begin, end);
    //TODO ��Ͽ飬 ������ṹ�����Ӧ������ṹ��ע�����v��
  }
  else {
    // ���ʽ���, ������������Ǳ��ʽ����һ�֡�
    ExpressionStmt* tmp = new ExpressionStmt();
    end = FindNextSemicolon(tokens, begin);
    tmp->root_ = ParseExpression(tokens, begin, end);
    ++end;
    result = tmp;
    return result;
  }
}

std::vector<std::pair<std::string, std::string>> ParseParameterList(const std::vector<Token>& tokens, size_t begin, size_t end) {
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  std::vector<std::pair<std::string, std::string>> result;
  size_t index = begin;
  while (true) {
    if (index == end) {
      break;
    }
    assert(tokens[begin].token == TOKEN::ID && tokens[index].type == Token::TYPE::STRING);
    assert(tokens[begin].token == TOKEN::ID && tokens[index + 1].type == Token::TYPE::STRING);
    result.push_back({ tokens[index].get<std::string>(), tokens[index + 1].get<std::string>() });
    index += 2;
    assert(tokens[index].token == TOKEN::COMMA);
    ++index;
    assert(index <= end);
  }
  return result;
}

// ����λ����ȷ��һ��id-TOKEN���������ơ��������ƻ��Ǳ�������.
// ��һ��TOKEN���������ƻ�������ʱ��ͨ���ڱ��������Ѱ�ң�ȷ����������͡�������
// ��һ��TOKEN�Ǳ�������ʱ��ͨ��������ϵͳ��֧�Ŷ�λ����λ�á�
Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Scopes::instance().EnterNewScope(Scope::TYPE::FUNC);
  Func result;
  assert(tokens[begin].token == TOKEN::FUNC);
  ++begin;
  //��������
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result.func_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  size_t match_parent = FindMatchedParenthesis(tokens, begin);
  ++begin;

  result.parameter_type_list_ = ParseParameterList(tokens, begin, match_parent);
  begin = match_parent + 1;
  //��������ֵ������������ϵͳ�п����ҵ���
  assert(tokens[begin].type == Token::TYPE::STRING && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
  result.return_type_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedBrace(tokens, begin);
  //������
  result.block_ = ParseBlockStmt(tokens, begin, match_brace);
  end = match_brace + 1;
  Scopes::instance().LeaveScope();
  return result;
}

Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Scopes::instance().EnterNewScope(Scope::TYPE::TYPE);
  Type result;
  assert(tokens[begin].token == TOKEN::TYPE);
  ++begin;
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result.type_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedParenthesis(tokens, begin);

  //TODO parse type-block.
  end = match_brace + 1;
  Scopes::instance().LeaveScope();
  return result;
}

void Parse(const std::vector<Token>& tokens) {
  size_t index = 0;
  while (true) {
    TOKEN current_token = tokens[index].token;
    if (current_token == TOKEN::TEOF) {
      break;
    }
    if (current_token == TOKEN::FUNC) {
      size_t next = 0;
      Func func = ParseFunc(tokens, index, next);
      index = next;
      assert(FuncSet::instance().Find(func.func_name_) == false);
      FuncSet::instance().Set(func.func_name_, func);
    }
    else if (current_token == TOKEN::TYPE) {
      size_t next = 0;
      Type type = ParseType(tokens, index, next);
      index = next;
      assert(TypeSet::instance().Find(type.type_name_) == false);
      TypeSet::instance().Set(type.type_name_, type);
    }
    else {
      size_t next = 0;
      //Variable v = ParseVariableDefinition(tokens, index, next);
      index = next;
      //TODO ȫ�ֵı�����¼�ṹע�����v��
    }
  }
}