#include "operator.h"

static void register_builtin_operators(std::unordered_map<TOKEN, Operator>& os) {
  Operator plus(2, TOKEN::PLUS);
  os[TOKEN::PLUS] = plus;
  Operator minus(2, TOKEN::MINUS);
  os[TOKEN::MINUS] = minus;
  Operator multiply(1, TOKEN::MULTIPLY);
  os[TOKEN::MULTIPLY] = multiply;
  Operator divide(1, TOKEN::DIVIDE);
  os[TOKEN::DIVIDE] = divide;
  
  Operator a(5, TOKEN::AND);
  Operator o(6, TOKEN::OR);
  Operator n(0, TOKEN::NOT);
  os[TOKEN::AND] = a;
  os[TOKEN::OR] = o;
  os[TOKEN::NOT] = n;

  Operator assign(7, TOKEN::ASSIGN);
  Operator compare(4, TOKEN::COMPARE);
  Operator greater_than(3, TOKEN::GREATER_THAN);
  Operator less_than(3, TOKEN::LESS_THAN);
  os[TOKEN::ASSIGN] = assign;
  os[TOKEN::COMPARE] = compare;
  os[TOKEN::GREATER_THAN] = greater_than;
  os[TOKEN::LESS_THAN] = less_than;
}

OperatorSet::OperatorSet() {
  register_builtin_operators(operators_);
}