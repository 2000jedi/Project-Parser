#pragma once

#include <string>
#include <vector>

#define TYPE_TREE std::string

class Node {
public:
  TYPE_TREE t;
  std::vector<Node> child;

  Node(TYPE_TREE x);

  void print();
};
