#pragma once
#include <vector>
#include "scanner.h"
#include "ast_node.h"

class Func {
 public:
  size_t func_name_index_;
  std::vector<std::pair<size_t, size_t>> parameter_type_index_list_;
  size_t return_type_index_;
  BlockStmt* block_;
};