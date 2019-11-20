#include <string>
#include <iostream>
#include "tree.hpp"

Node::Node(TYPE_TREE x) {
  this->t = x;
  child.clear();
}

void tree_print_rec(Node node, int indent) {
  int i;
  for (i = 0; i < indent; ++i) printf("  ");
  std::cout << node.t << std::endl;

  if (node.t.substr(0, 9) != "<LITERAL_") {
    for (auto i = node.child.begin(); i != node.child.end(); ++i) {
      tree_print_rec(*i, indent + 1);
    }
  }
}

void Node::print() {
  tree_print_rec(*this, 0);
}
