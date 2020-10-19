#include "operator.h"

//目前仅仅注册供静态分析使用的函数映射表，提供返回类型。
static void register_builtin_plus_operators(Operator& plus) {
  plus.static_maps_[{"string", "string"}] = "string";
  plus.static_maps_[{"int", "int"}] = "int";
  plus.static_maps_[{"double", "double" }] = "double";
  plus.static_maps_[{"int", "double"}] = "double";
  plus.static_maps_[{"double", "int"}] = "double";
  plus.static_maps_[{"char", "char"}] = "string";
}

static void register_builtin_minus_operators(Operator& minus) {
  minus.static_maps_[{"int", "int"}] = "int";
  minus.static_maps_[{"double", "double" }] = "double";
  minus.static_maps_[{"int", "double"}] = "double";
  minus.static_maps_[{"double", "int"}] = "double";
}

static void register_builtin_multiply_operators(Operator& op) {
  op.static_maps_[{"int", "int"}] = "int";
  op.static_maps_[{"double", "double" }] = "double";
  op.static_maps_[{"int", "double"}] = "double";
  op.static_maps_[{"double", "int"}] = "double";
}

static void register_builtin_divide_operators(Operator& op) {
  op.static_maps_[{"int", "int"}] = "int";
  op.static_maps_[{"double", "double" }] = "double";
  op.static_maps_[{"int", "double"}] = "double";
  op.static_maps_[{"double", "int"}] = "double";
}

static void register_builtin_and_operators(Operator& op) {
  op.static_maps_[{"bool", "bool"}] = "bool";
}

static void register_builtin_or_operators(Operator& op) {
  op.static_maps_[{"bool", "bool"}] = "bool";
}

static void register_builtin_not_operators(Operator& op) {
  op.static_maps_[{"bool", "bool"}] = "bool";
}

static void register_builtin_operators(std::unordered_map<TOKEN, Operator>& os) {
  Operator plus(2, TOKEN::PLUS);
  register_builtin_plus_operators(plus);
  os[TOKEN::PLUS] = plus;

  Operator minus(2, TOKEN::MINUS);
  register_builtin_minus_operators(minus);
  os[TOKEN::MINUS] = minus;

  Operator multiply(1, TOKEN::MULTIPLY);
  register_builtin_multiply_operators(multiply);
  os[TOKEN::MULTIPLY] = multiply;

  Operator divide(1, TOKEN::DIVIDE);
  register_builtin_divide_operators(divide);
  os[TOKEN::DIVIDE] = divide;
  
  Operator a(5, TOKEN::AND);
  register_builtin_and_operators(a);
  Operator o(6, TOKEN::OR);
  register_builtin_or_operators(o);
  Operator n(0, TOKEN::NOT);
  register_builtin_not_operators(n);
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