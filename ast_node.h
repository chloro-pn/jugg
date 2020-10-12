#pragma once

class AstNode {
 public:
  explicit AstNode(int line, int column) : line(line), column(column) {

  }

  virtual ~AstNode() = default;

 private:
  int line;
  int column;
};

class Expression : public AstNode {
  using AstNode::AstNode;

 public:
   virtual ~Expression() = default;
};