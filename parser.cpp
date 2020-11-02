#include "parser.h"
#include "func.h"
#include <cassert>
#include <algorithm>

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
#define FindNext(FuncName, x) \
size_t FuncName(const std::vector<Token>& tokens, size_t begin) { \
  size_t index = begin; \
  while (index < tokens.size()) { \
    if(tokens[index].token == x) { \
      return index; \
    } \
    ++index; \
  } \
  return -1; \
}

FindNext(FindNextSemicolon, TOKEN::SEMICOLON);

FindNext(FindNextComma, TOKEN::COMMA);

size_t FindNextCommaOrEnd(const std::vector<Token>& tokens, size_t begin, size_t end) {
  size_t index = begin;
  while (index < tokens.size()) {
    if (tokens[index].token == TOKEN::COMMA || index == end) {
      return index;
    }
    ++index;
  }
  return -1;
}

ComprehensiveType ParseComprehensiveType(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  assert(tokens[begin].type == Token::TYPE::STRING);
  assert(TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
  ComprehensiveType result;
  result.base_type_ = tokens[begin].get<std::string>();
  ++begin;
  while (true) {
    if (tokens[begin].token == TOKEN::MULTIPLY) {
      result.modifiers_.push_back(ComprehensiveType::Modifier::Pointer);
      ++begin;
      continue;
    }
    else if (tokens[begin].token == TOKEN::LEFT_BRACKETS) {
      ++begin;
      assert(tokens[begin].token == TOKEN::RIGHT_BRACKETS);
      ++begin;
      result.modifiers_.push_back(ComprehensiveType::Modifier::Array);
      continue;
    }
    break;
  }
  end = begin;
  return result;
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
  assert(FuncSet::instance().Find(result->func_name_) == true || Interpreter::instance().FindInnerFunc(result->func_name_) == true);
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  if (begin + 1 != end) {
    while (true) {
      if (begin == end) {
        break;
      }
      ++begin;
      size_t next = FindNextCommaOrEnd(tokens, begin, end);
      // func_name(xxxxx, xxxxx, xxxx)
      //           |    |
      //         begin end
      result->parameters_.push_back(ParseExpression(tokens, begin, next));
      begin = next;
    }
  }
  return result;
}

MethodCallExpr* ParseMethodCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end) {
  MethodCallExpr* result = new MethodCallExpr;
  assert(tokens[begin].token == TOKEN::ID);
  assert(tokens[begin].type == Token::TYPE::STRING);
  result->var_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::DECIMAL_POINT);
  ++begin;
  assert(tokens[begin].token == TOKEN::ID);
  assert(tokens[begin].type == Token::TYPE::STRING);
  result->method_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  if (begin + 1 != end) {
    while (true) {
      if (begin == end) {
        break;
      }
      ++begin;
      size_t next = FindNextCommaOrEnd(tokens, begin, end);
      // func_name(xxxxx, xxxxx, xxxx)
      //           |    |
      //         begin end
      result->parameters_.push_back(ParseExpression(tokens, begin, next));
      begin = next;
    }
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
  std::stack<TOKEN> unary_operators;
  for (size_t i = begin; i < end; ++i) {
    TOKEN current_token = tokens[i].token;
    // ���������⴦����Ϊ��ı����������ȼ�
    if (current_token == TOKEN::LEFT_PARENTHESIS) {
      size_t match_parent = FindMatchedParenthesis(tokens, i);
      // �ݹ���������еı��ʽ
      operands.push(ParseExpression(tokens, i, match_parent));
      i = match_parent;
      continue;
    }
    // ���е�������TOKENӦ�ñ�����
    assert(current_token != TOKEN::RIGHT_PARENTHESIS);
    // ���������
    if (OperatorSet::instance().Find(current_token) == false) {
      Expression* expr = nullptr;
      //�������ñ��ʽ
      if (tokens[i].type == Token::TYPE::STRING && (
        FuncSet::instance().Find(tokens[i].get<std::string>()) == true || 
        Interpreter::instance().FindInnerFunc(tokens[i].get<std::string>()) == true)) {
        assert(tokens[i + 1].token == TOKEN::LEFT_PARENTHESIS);
        size_t match_parent = FindMatchedParenthesis(tokens, i + 1);
        expr = ParseFuncCallExpr(tokens, i, match_parent);
        if (unary_operators.empty() == false) {
          UnaryExpr* tmp = new UnaryExpr;
          tmp->child_ = expr;
          tmp->operator_token_ = unary_operators.top();
          unary_operators.pop();
          expr = tmp;
        }
        i = match_parent;
      }
      //��Ա���ʱ��ʽ
      else if (i + 1 < tokens.size() && tokens[i + 1].token == TOKEN::DECIMAL_POINT) {
        //�������� a  .  bcd  ( xxx )...
        //         i i+1 i+2 i+3
        if (i + 3 < tokens.size() && tokens[i + 3].token == TOKEN::LEFT_PARENTHESIS) {
          size_t match_parent = FindMatchedParenthesis(tokens, i + 3);
          expr = ParseMethodCallExpr(tokens, i, match_parent);
          if (unary_operators.empty() == false) {
            UnaryExpr* tmp = new UnaryExpr;
            tmp->child_ = expr;
            tmp->operator_token_ = unary_operators.top();
            unary_operators.pop();
            expr = tmp;
          }
          i = match_parent;
        }
        //���ݳ�Ա����
        else {
          DataMemberExpr* dexpr = new DataMemberExpr;
          assert(tokens[i].token == TOKEN::ID);
          dexpr->var_name_ = tokens[i].get<std::string>();
          ++i;
          assert(tokens[i].token == TOKEN::DECIMAL_POINT);
          ++i;
          assert(tokens[i].token == TOKEN::ID);
          dexpr->data_member_name_ = tokens[i].get<std::string>();
          expr = dexpr;
          if (unary_operators.empty() == false) {
            UnaryExpr* tmp = new UnaryExpr;
            tmp->child_ = expr;
            tmp->operator_token_ = unary_operators.top();
            unary_operators.pop();
            expr = tmp;
          }
        }
      }
      else {
        if (tokens[i].token == TOKEN::TRUE || tokens[i].token == TOKEN::FALSE) {
          BoolIteralExpr* e = new BoolIteralExpr;
          e->b_ = tokens[i].token == TOKEN::TRUE ? true : false;
          expr = e;
        }
        else if (tokens[i].token == TOKEN::STRING_ITERAL) {
          StringIteralExpr* e = new StringIteralExpr;
          e->str_ = tokens[i].get<std::string>();
          expr = e;
        }
        else if (tokens[i].token == TOKEN::DOUBLE_ITERAL) {
          DoubleIteralExpr* e = new DoubleIteralExpr;
          e->d_ = tokens[i].get<double>();
          expr = e;
        }
        else if (tokens[i].token == TOKEN::INT_ITERAL) {
          IntIteralExpr* e = new IntIteralExpr;
          e->int_ = tokens[i].get<int64_t>();
          expr = e;
        }
        else if (tokens[i].token == TOKEN::BYTE_ITERAL) {
          ByteIteralExpr* e = new ByteIteralExpr;
          e->c_ = tokens[i].get<uint8_t>();
          expr = e;
        }
        else {
          assert(tokens[i].token == TOKEN::ID);
          IdExpr* idexpr = new IdExpr;
          //ͨ��id_name_, type_name_��scope_index_������O(1)ʱ���ڶ�λ��������
          idexpr->id_name_ = tokens[i].get<std::string>();
          expr = idexpr;
        }
        if (unary_operators.empty() == false) {
          UnaryExpr* tmp = new UnaryExpr;
          tmp->child_ = expr;
          tmp->operator_token_ = unary_operators.top();
          unary_operators.pop();
          expr = tmp;
        }
      }
      //�Ƿ���token
      assert(expr != nullptr);
      operands.push(expr);
    }
    else {
      // ������ڵ�һ��λ�û���ǰһ��λ��Ҳ������������ǵ�Ԫ�����
      if (i == begin || OperatorSet::instance().Find(tokens[i - 1].token) == true) {
        assert(OperatorSet::instance().FindUnary(current_token) == true);
        unary_operators.push(current_token);
        continue;
      }
      // ˫Ԫ��������ܿ�����Ԫ�����
      assert(unary_operators.empty() == true);
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

FMBlockStmt* ParseFMBlockStmt(const std::vector<Token>& tokens, size_t begin, size_t end) {
  FMBlockStmt* result = new FMBlockStmt();
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
    ReturnStmt* result = new ReturnStmt;
    ++begin;
    if (tokens[begin].token != TOKEN::SEMICOLON) {
      size_t next = FindNextSemicolon(tokens, begin);
      result->return_exp_ = ParseExpression(tokens, begin, next);
      end = next + 1;
    }
    else {
      result->return_exp_ = nullptr;
      end = begin + 1;
    }
    return result;
  }
  else if (tokens[begin].token == TOKEN::CONTINUE) {
    ++begin;
    assert(tokens[begin].token == TOKEN::SEMICOLON);
    end = begin + 1;
    return new ContinueStmt;
  }
  else if (tokens[begin].token == TOKEN::BREAK) {
    ++begin;
    assert(tokens[begin].token == TOKEN::SEMICOLON);
    end = begin + 1;
    return new BreakStmt;
  }
  else if (tokens[begin].type == Token::TYPE::STRING && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true) {
    //�����������
    VariableDefineStmt* v = ParseVariableDefinition(tokens, begin, end);
    return v;
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

std::vector<std::pair<std::string, ComprehensiveType>> ParseParameterList(const std::vector<Token>& tokens, size_t begin, size_t end) {
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  std::vector<std::pair<std::string, ComprehensiveType>> result;
  if (begin + 1 == end) {
    return result;
  }
  size_t index = begin;
  while (true) {
    if (index == end) {
      break;
    }
    size_t next = 0;
    ComprehensiveType ct = ParseComprehensiveType(tokens, index, next);
    index = next;
    assert(tokens[index].token == TOKEN::ID && tokens[index].type == Token::TYPE::STRING);
    std::string var_name = tokens[index].get<std::string>();
    result.push_back({ var_name, ct });
    ++index;
    assert(tokens[index].token == TOKEN::COMMA || tokens[index].token == TOKEN::RIGHT_PARENTHESIS);
    if (tokens[index].token == TOKEN::COMMA) {
      ++index;
    }
    assert(index <= end);
  }
  return result;
}

// ����λ����ȷ��һ��id-TOKEN���������ơ��������ƻ��Ǳ�������.
// ��һ��TOKEN���������ƻ�������ʱ��ͨ���ڱ��������Ѱ�ң�ȷ����������͡�������
// ��һ��TOKEN�Ǳ�������ʱ��ͨ��������ϵͳ��֧�Ŷ�λ����λ�á�
Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end) {
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

  //TODO : �ò����б����Scope�еı�����
  result.parameter_type_list_ = ParseParameterList(tokens, begin, match_parent);

  begin = match_parent + 1;

  //��������ֵ������������ϵͳ�п����ҵ���
  assert(tokens[begin].type == Token::TYPE::STRING && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
  result.return_type_.base_type_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedBrace(tokens, begin);
  //������
  result.block_ = ParseFMBlockStmt(tokens, begin, match_brace);
  end = match_brace + 1;
  return result;
}

//TODO
Method ParseMethod(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Method result;
  assert(tokens[begin].token == TOKEN::METHOD);
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  ++begin;
  assert(tokens[begin].type == Token::TYPE::STRING);
  result.type_name_ = tokens[begin].get<std::string>();
  assert(TypeSet::instance().Find(result.type_name_) == true);
  ++begin;
  assert(tokens[begin].token == TOKEN::RIGHT_PARENTHESIS);

  ++begin;
  //��������
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result.method_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  size_t match_parent = FindMatchedParenthesis(tokens, begin);
  ++begin;

  result.parameter_type_list_ = ParseParameterList(tokens, begin, match_parent);
  begin = match_parent + 1;

  //��������ֵ������������ϵͳ�п����ҵ���
  assert(tokens[begin].type == Token::TYPE::STRING && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
  result.return_type_.base_type_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedBrace(tokens, begin);
  //������
  result.block_ = ParseFMBlockStmt(tokens, begin, match_brace);
  end = match_brace + 1;
  return result;
}

// type���壺
// type type_name {
//   int age;
//   string name;
// }
Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Type result;
  assert(tokens[begin].token == TOKEN::TYPE);
  ++begin;
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result.type_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedBrace(tokens, begin);

  end = match_brace;
  ++begin;
  while (true) {
    if (begin == end) {
      break;
    }
    size_t next = 0;
    ComprehensiveType ct = ParseComprehensiveType(tokens, begin, next);
    begin = next;
    assert(tokens[begin].type == Token::TYPE::STRING);
    std::string var_name = tokens[begin].get<std::string>();
    result.RegisterData(var_name, ct);
    ++begin;
    assert(tokens[begin].token == TOKEN::SEMICOLON);
    ++begin;
  }
  ++end;
  return result;
}

// TODO:
// ���������﷨:
// type_name var(expr, expr, expr, ...);
// ����Ƿ���������ݳ�Ա������ƥ�䡣
VariableDefineStmt* ParseVariableDefinition(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  VariableDefineStmt* result = new VariableDefineStmt;
  size_t next = 0;
  result->type_name_ = ParseComprehensiveType(tokens, begin, next);
  begin = next;
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result->var_name_ = tokens[begin].get<std::string>();
  ++begin;
  if (tokens[begin].token == TOKEN::SEMICOLON) {
    end = begin + 1;
    return result;
  }
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  size_t match_parent = FindMatchedParenthesis(tokens, begin);
  if (begin + 1 != match_parent) {
    while (true) {
      if (begin == match_parent) {
        break;
      }
      ++begin;
      size_t next = FindNextCommaOrEnd(tokens, begin, match_parent);
      // variable_name(xxxxx, xxxxx, xxxx)
      //           |    |
      //         begin end
      result->constructors_.push_back(ParseExpression(tokens, begin, next));
      begin = next;
    }
  }
  ++begin;
  assert(tokens[begin].token == TOKEN::SEMICOLON);
  end = begin + 1;
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
      FuncSet::instance().RegisterFunc(func);
    }
    else if (current_token == TOKEN::TYPE) {
      size_t next = 0;
      Type type = ParseType(tokens, index, next);
      index = next;
      TypeSet::instance().RegisterType(type);
    }
    else if (current_token == TOKEN::METHOD) {
      size_t next = 0;
      Method method = ParseMethod(tokens, index, next);
      index = next;
      assert(TypeSet::instance().Find(method.type_name_) == true);
      TypeSet::instance().Get(method.type_name_).RegisterMethod(method);
    }
    else {
      size_t next = 0;
      VariableDefineStmt* vs = ParseVariableDefinition(tokens, index, next);
      Interpreter::instance().RegisterGlobalVariable(vs);
      index = next;
    }
  }
}