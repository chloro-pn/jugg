#include "operator.h"

static Variable* plus_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  int64_t v = static_cast<IntVariable*>(v1)->val_ + static_cast<IntVariable*>(v2)->val_;
  IntVariable* result = new IntVariable;
  result->type_name_.base_type_ = "int";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* plus_string_string(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "string" && v2->type_name_.base_type_ == "string");
  std::string v = static_cast<StringVariable*>(v1)->val_ + static_cast<StringVariable*>(v2)->val_;
  StringVariable* result = new StringVariable;
  result->type_name_.base_type_ = "string";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* plus_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  double v = static_cast<DoubleVariable*>(v1)->val_ + static_cast<DoubleVariable*>(v2)->val_;
  DoubleVariable* result = new DoubleVariable;
  result->type_name_.base_type_ = "double";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_plus_operators(BinaryOperator& plus) {
  plus.op_funcs_[{"string", "string"}] = plus_string_string;
  plus.op_funcs_[{"int", "int"}] = plus_int_int;
  plus.op_funcs_[{"double", "double"}] = plus_double_double;
}

static Variable* minus_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  int64_t v = static_cast<IntVariable*>(v1)->val_ - static_cast<IntVariable*>(v2)->val_;
  IntVariable* result = new IntVariable;
  result->type_name_.base_type_ = "int";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* minus_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  double v = static_cast<DoubleVariable*>(v1)->val_ - static_cast<DoubleVariable*>(v2)->val_;
  DoubleVariable* result = new DoubleVariable;
  result->type_name_.base_type_ = "double";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_minus_operators(BinaryOperator& minus) {
  minus.op_funcs_[{"int", "int"}] = minus_int_int;
  minus.op_funcs_[{"double", "double"}] = minus_double_double;
}

static Variable* multiply_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  int64_t v = static_cast<IntVariable*>(v1)->val_ * static_cast<IntVariable*>(v2)->val_;
  IntVariable* result = new IntVariable;
  result->type_name_.base_type_ = "int";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* multiply_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  double v = static_cast<DoubleVariable*>(v1)->val_ * static_cast<DoubleVariable*>(v2)->val_;
  DoubleVariable* result = new DoubleVariable;
  result->type_name_.base_type_ = "double";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_multiply_operators(BinaryOperator& op) {
  op.op_funcs_[{"int", "int"}] = multiply_int_int;
  op.op_funcs_[{"double", "double"}] = multiply_double_double;
}

static Variable* divide_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  int64_t v = static_cast<IntVariable*>(v1)->val_ / static_cast<IntVariable*>(v2)->val_;
  IntVariable* result = new IntVariable;
  result->type_name_.base_type_ = "int";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* divide_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  double v = static_cast<DoubleVariable*>(v1)->val_ / static_cast<DoubleVariable*>(v2)->val_;
  DoubleVariable* result = new DoubleVariable;
  result->type_name_.base_type_ = "double";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_divide_operators(BinaryOperator& op) {
  op.op_funcs_[{"int", "int"}] = divide_int_int;
  op.op_funcs_[{"double", "double"}] = divide_double_double;
}

static Variable* and_bool_bool(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "bool" && v2->type_name_.base_type_ == "bool");
  bool b = static_cast<BoolVariable*>(v1)->val_ && static_cast<BoolVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_= "bool";
  result->val_ = b;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_and_operators(BinaryOperator& op) {
  op.op_funcs_[{"bool", "bool"}] = and_bool_bool;
}

static Variable* or_bool_bool(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "bool" && v2->type_name_.base_type_ == "bool");
  bool b = static_cast<BoolVariable*>(v1)->val_ || static_cast<BoolVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->val_ = b;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_or_operators(BinaryOperator& op) {
  op.op_funcs_[{"bool", "bool"}] = or_bool_bool;
}

static void register_builtin_assign_operators(BinaryOperator& op) {
  auto assign_func = [](Variable* v1, Variable* v2)->Variable* {
    v1->Assign(v2);
    assert(v1->cate_ == Variable::Category::Lvalue);
    return v1;
  };
  op.op_funcs_[{"int", "int"}] = assign_func;
  op.op_funcs_[{"double", "double"}] = assign_func;
  op.op_funcs_[{"double", "int"}] = assign_func;
  op.op_funcs_[{"string", "string"}] = assign_func;
  op.op_funcs_[{"bool", "bool"}] = assign_func;
  op.op_funcs_[{"byte", "byte"}] = assign_func;
}

static Variable* compare_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  int64_t n1 = static_cast<IntVariable*>(v1)->val_;
  int64_t n2 = static_cast<IntVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->cate_ = Variable::Category::Rvalue;
  result->type_name_.base_type_ = "bool";
  if (n1 == n2) {
    result->val_ = true;
  }
  else {
    result->val_ = false;
  }
  result->id_name_ = "tmp";
  return result;
}

static Variable* compare_string_string(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "string" && v2->type_name_.base_type_ == "string");
  std::string& s1 = static_cast<StringVariable*>(v1)->val_;
  std::string& s2 = static_cast<StringVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->cate_ = Variable::Category::Rvalue;
  result->type_name_.base_type_ = "bool";
  if (s1 == s2) {
    result->val_ = true;
  }
  else {
    result->val_ = false;
  }
  result->id_name_ = "tmp";
  return result;
}

static Variable* compare_byte_byte(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "byte" && v2->type_name_.base_type_ == "byte");
  uint8_t c1 = static_cast<ByteVariable*>(v1)->val_;
  uint8_t c2 = static_cast<ByteVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->cate_ = Variable::Category::Rvalue;
  result->type_name_.base_type_ = "bool";
  if (c1 == c2) {
    result->val_ = true;
  }
  else {
    result->val_ = false;
  }
  result->id_name_ = "tmp";
  return result;
}

static Variable* compare_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  double d1 = static_cast<DoubleVariable*>(v1)->val_;
  double d2 = static_cast<DoubleVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->cate_ = Variable::Category::Rvalue;
  result->type_name_.base_type_ = "bool";
  if (d1 == d2) {
    result->val_ = true;
  }
  else {
    result->val_ = false;
  }
  result->id_name_ = "tmp";
  return result;
}

static Variable* compare_bool_bool(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "bool" && v2->type_name_.base_type_ == "bool");
  bool b1 = static_cast<BoolVariable*>(v1)->val_;
  bool b2 = static_cast<BoolVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->cate_ = Variable::Category::Rvalue;
  result->type_name_.base_type_ = "bool";
  if (b1 == b2) {
    result->val_ = true;
  }
  else {
    result->val_ = false;
  }
  result->id_name_ = "tmp";
  return result;
}

static void register_builtin_compare_operators(BinaryOperator& op) {
  op.op_funcs_[{"int", "int"}] = compare_int_int;
  op.op_funcs_[{"string", "string"}] = compare_string_string;
  op.op_funcs_[{"double", "double"}] = compare_double_double;
  op.op_funcs_[{"byte", "byte"}] = compare_byte_byte;
  op.op_funcs_[{"bool", "bool"}] = compare_bool_bool;
}

static Variable* gt_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  bool v = static_cast<IntVariable*>(v1)->val_ > static_cast<IntVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* gt_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  bool v = static_cast<DoubleVariable*>(v1)->val_ > static_cast<DoubleVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_greater_than_operators(BinaryOperator& op) {
  op.op_funcs_[{"int", "int"}] = gt_int_int;
  op.op_funcs_[{"double", "double"}] = gt_double_double;
}

static Variable* lt_int_int(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "int" && v2->type_name_.base_type_ == "int");
  bool v = static_cast<IntVariable*>(v1)->val_ < static_cast<IntVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static Variable* lt_double_double(Variable* v1, Variable* v2) {
  assert(v1->type_name_.base_type_ == "double" && v2->type_name_.base_type_ == "double");
  bool v = static_cast<DoubleVariable*>(v1)->val_ < static_cast<DoubleVariable*>(v2)->val_;
  BoolVariable* result = new BoolVariable;
  result->type_name_.base_type_ = "bool";
  result->val_ = v;
  result->cate_ = Variable::Category::Rvalue;
  return result;
}

static void register_builtin_less_than_operators(BinaryOperator& op) {
  op.op_funcs_[{"int", "int"}] = lt_int_int;
  op.op_funcs_[{"double", "double"}] = lt_double_double;
}

static void register_builtin_operators(std::unordered_map<TOKEN, BinaryOperator>& os) {
  BinaryOperator plus(2, TOKEN::PLUS);
  register_builtin_plus_operators(plus);
  os[TOKEN::PLUS] = plus;

  BinaryOperator minus(2, TOKEN::MINUS);
  register_builtin_minus_operators(minus);
  os[TOKEN::MINUS] = minus;

  BinaryOperator multiply(1, TOKEN::MULTIPLY);
  register_builtin_multiply_operators(multiply);
  os[TOKEN::MULTIPLY] = multiply;

  BinaryOperator divide(1, TOKEN::DIVIDE);
  register_builtin_divide_operators(divide);
  os[TOKEN::DIVIDE] = divide;
  
  BinaryOperator a(5, TOKEN::AND);
  register_builtin_and_operators(a);
  BinaryOperator o(6, TOKEN::OR);
  register_builtin_or_operators(o);
  os[TOKEN::AND] = a;
  os[TOKEN::OR] = o;

  BinaryOperator assign(7, TOKEN::ASSIGN);
  register_builtin_assign_operators(assign);

  BinaryOperator compare(4, TOKEN::COMPARE);
  register_builtin_compare_operators(compare);

  BinaryOperator greater_than(3, TOKEN::GREATER_THAN);
  register_builtin_greater_than_operators(greater_than);
  BinaryOperator less_than(3, TOKEN::LESS_THAN);
  register_builtin_less_than_operators(less_than);
  os[TOKEN::ASSIGN] = assign;
  os[TOKEN::COMPARE] = compare;
  os[TOKEN::GREATER_THAN] = greater_than;
  os[TOKEN::LESS_THAN] = less_than;
}

void register_builtin_uoperators(std::unordered_map<TOKEN, UnaryOperator>& os) {
  UnaryOperator n;
  n.token_ = TOKEN::NOT;
  n.func_ = [](Variable* v)->Variable* {
    assert(v->type_name_.base_type_ == "bool");
    BoolVariable* result = new BoolVariable;
    result->cate_ = Variable::Category::Rvalue;
    result->id_name_ = "tmp";
    result->type_name_ = v->type_name_;
    result->val_ = !(static_cast<BoolVariable*>(v)->val_);
    return result;
  };
  os[TOKEN::NOT] = n;

  UnaryOperator deref;
  deref.token_ = TOKEN::MULTIPLY;
  deref.func_ = [](Variable* v)->Variable* {
    assert(v->type_name_.modifiers_.empty() == false);
    assert(v->type_name_.modifiers_.back() == ComprehensiveType::Modifier::Pointer);
    return static_cast<PointerVariable*>(v)->ptr_;
  };
  os[TOKEN::MULTIPLY] = deref;
}

OperatorSet::OperatorSet() {
  register_builtin_operators(boperators_);
  register_builtin_uoperators(uoperators_);
}