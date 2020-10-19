#include "parser.h"
#include "func.h"
#include "scope.h"
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

//因为要处理空语句，因此begin本身可能就是分号。
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

FuncCallExpr* ParseFuncCallExpr(const std::vector<Token>& tokens, size_t begin, size_t end) {
  FuncCallExpr* result = new FuncCallExpr;
  assert(tokens[begin].token == TOKEN::ID);
  assert(tokens[begin].type == Token::TYPE::STRING);
  result->func_name_ = tokens[begin].get<std::string>();
  //确保当前函数可以被找到，即已经在前面解析过程中被定义。
  assert(FuncSet::instance().Find(result->func_name_) == true);
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  assert(tokens[end].token == TOKEN::RIGHT_PARENTHESIS);
  while (true) {
    if (begin == end) {
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

// end是表达式最后一个token的下一个token索引值。
// TODO : 处理函数调用表达式和下标访问表达式，成员访问表达式。
Expression* ParseExpression(const std::vector<Token>& tokens, size_t begin, size_t end) {
  if (begin == end) {
    return nullptr; // 可能是空语句
  }
  std::stack<Expression*> operands;
  std::stack<TOKEN> operators;
  for (size_t i = begin; i < end; ++i) {
    TOKEN current_token = tokens[i].token;
    // 左括号特殊处理，因为其改变了运算优先级
    if (current_token == TOKEN::LEFT_PARENTHESIS) {
      size_t match_parent = FindMatchedParenthesis(tokens, i);
      // 递归求解括号中的表达式
      operands.push(ParseExpression(tokens, i, match_parent));
      i = match_parent;
      continue;
    }
    // 所有的右括号TOKEN应该被跳过
    assert(current_token != TOKEN::RIGHT_PARENTHESIS);
    if (OperatorSet::instance().find(current_token) == false) {
      //函数调用语句
      if (tokens[i].type == Token::TYPE::STRING && FuncSet::instance().Find(tokens[i].get<std::string>()) == true) {
        assert(tokens[i + 1].token == TOKEN::LEFT_PARENTHESIS);
        size_t match_parent = FindMatchedParenthesis(tokens, i + 1);
        operands.push(ParseFuncCallExpr(tokens, i, match_parent));
        i = match_parent;
      }
      else {
        //操作数，在构造处执行静态绑定
        assert(tokens[i].token == TOKEN::ID);
        IdExpr* idexpr = new IdExpr;
        idexpr->id_name_ = tokens[i].get<std::string>();
        idexpr->scope_index_ = Scopes::instance().GetCurrentScope().index_;
        idexpr->var_ = Scopes::instance().VariableStaticBinding(idexpr->id_name_, idexpr->scope_index_);
        operands.push(idexpr);
      }
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
    ReturnStmt* result = new ReturnStmt;
    ++begin;
    if (tokens[begin].token != TOKEN::SEMICOLON) {
      size_t next = FindNextSemicolon(tokens, begin);
      result->return_var_ = ParseExpression(tokens, begin, next);
      end = next + 1;
    }
    else {
      result->return_var_ = nullptr;
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
  else if (tokens[begin].token == TOKEN::ID && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true) {
    //变量定义语句
    VariableDefineStmt* v = ParseVariableDefinition(tokens, begin, end);
    //TODO 结合块， 作用域结构，向对应作用域结构中注册变量v。
    assert(Scopes::instance().GetCurrentScope().Find(v->var_->variable_name_) == false);
    Scopes::instance().GetCurrentScope().vars_[v->var_->variable_name_] = v->var_;
    return v;
  }
  else {
    // 表达式语句, 函数调用语句是表达式语句的一种。
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
    assert(tokens[index].type == Token::TYPE::STRING);
    assert(tokens[index + 1].token == TOKEN::ID && tokens[index + 1].type == Token::TYPE::STRING);
    result.push_back({ tokens[index].get<std::string>(), tokens[index + 1].get<std::string>() });
    index += 2;
    assert(tokens[index].token == TOKEN::COMMA || tokens[index].token == TOKEN::RIGHT_PARENTHESIS);
    if (tokens[index].token == TOKEN::COMMA) {
      ++index;
    }
    assert(index <= end);
  }
  return result;
}

// 根据位置来确定一个id-TOKEN是类型名称、函数名称还是变量名称.
// 当一个TOKEN是类型名称或函数名称时，通过在编译程序中寻找，确定具体的类型、函数。
// 当一个TOKEN是变量名称时，通过作用域系统的支撑定位变量位置。
Func ParseFunc(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Scopes::instance().EnterNewScope(Scope::TYPE::FUNC);
  Func result;
  assert(tokens[begin].token == TOKEN::FUNC);
  ++begin;
  //函数名称
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result.func_name_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  size_t match_parent = FindMatchedParenthesis(tokens, begin);
  ++begin;

  //每个Func对象都有一个对应的Scope绑定。
  result.scope_index_ = Scopes::instance().GetCurrentScope().index_;

  //TODO : 用参数列表填充Scope中的变量表。
  result.parameter_type_list_ = ParseParameterList(tokens, begin, match_parent);
  for (auto& each : result.parameter_type_list_) {
    assert(Scopes::instance().GetCurrentScope().Find(each.second) == false);
    //不在这里进行变量初始化，由解释器填充。
    Scopes::instance().GetCurrentScope().vars_[each.second] = CreateVariable(each.second);
  }
  begin = match_parent + 1;

  //函数返回值的类型在类型系统中可以找到。
  assert(tokens[begin].type == Token::TYPE::STRING && TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
  result.return_type_ = tokens[begin].get<std::string>();
  ++begin;
  assert(tokens[begin].token == TOKEN::LEFT_BRACE);
  size_t match_brace = FindMatchedBrace(tokens, begin);
  //函数体
  result.block_ = ParseBlockStmt(tokens, begin, match_brace);
  end = match_brace + 1;
  Scopes::instance().LeaveScope();
  return result;
}

// type定义：
// type type_name {
//   int age;
//   string name;
//   func GetName() string {
//     return name;
//   }
// }
Type ParseType(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  Scopes::instance().EnterNewScope(Scope::TYPE::TYPE);
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
    if (tokens[begin].token == TOKEN::FUNC) {
      size_t next = 0;
      Func func = ParseFunc(tokens, begin, next);
      assert(result.FindMethod(func.func_name_) == false);
      result.methods_[func.func_name_] = func;
      begin = next;
    }
    else {
      assert(tokens[begin].type == Token::TYPE::STRING);
      assert(TypeSet::instance().Find(tokens[begin].get<std::string>()) == true);
      std::string type_name = tokens[begin].get<std::string>();
      ++begin;
      assert(tokens[begin].type == Token::TYPE::STRING);
      std::string var_name = tokens[begin].get<std::string>();
      assert(result.FindData(var_name) == false);
      result.datas_[var_name] = type_name;
      ++begin;
      assert(tokens[begin].token == TOKEN::SEMICOLON);
      ++begin;
    }
  }
  ++end;
  Scopes::instance().LeaveScope();
  return result;
}

// TODO:
// 变量定义语法:
// type_name var(expr, expr, expr, ...);
// 检查是否与变量数据成员的类型匹配。
VariableDefineStmt* ParseVariableDefinition(const std::vector<Token>& tokens, size_t begin, size_t& end) {
  VariableDefineStmt* result = new VariableDefineStmt;
  assert(tokens[begin].type == Token::TYPE::STRING);
  std::string type_name = tokens[begin].get<std::string>();
  result->var_ = CreateVariable(type_name);
  ++begin;
  assert(tokens[begin].token == TOKEN::ID && tokens[begin].type == Token::TYPE::STRING);
  result->var_->variable_name_ = tokens[begin].get<std::string>();
  ++begin;
  if (tokens[begin].token == TOKEN::SEMICOLON) {
    end = begin + 1;
    return result;
  }
  assert(tokens[begin].token == TOKEN::LEFT_PARENTHESIS);
  size_t match_parent = FindMatchedParenthesis(tokens, begin);
  while (true) {
    if (begin == match_parent) {
      break;
    }
    ++begin;
    size_t next = FindNextCommaOrRightParenthesis(tokens, begin);
    // variable_name(xxxxx, xxxxx, xxxx)
    //           |    |
    //         begin end
    result->constructors_.push_back(ParseExpression(tokens, begin, next));
    begin = next;
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
      //[done] vs应该被记录在顶级作用域中。
      VariableDefineStmt* vs = ParseVariableDefinition(tokens, index, next);
      Variable* v = vs->var_;
      assert(Scopes::instance().GetCurrentScope().index_ == 0);
      assert(Scopes::instance().GetCurrentScope().Find(v->variable_name_) == false);
      Scopes::instance().GetCurrentScope().vars_[v->variable_name_] = v;
      Scopes::instance().GetGlobalVariableStmt().push_back(vs);
      index = next;
    }
  }
}