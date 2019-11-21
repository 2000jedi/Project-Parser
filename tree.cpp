#include <string>
#include <iostream>
#include "tree.hpp"

template <class T> 
Node<T>::Node(T x) {
  this->t = x;
  child.clear();
}

template <class T> 
void tree_print_rec(Node<T> node, int indent) {
  int i;
  for (i = 0; i < indent; ++i) printf("  ");
  std::cout << node.t << std::endl;

  for (auto i = node.child.begin(); i != node.child.end(); ++i) {
    tree_print_rec(*i, indent + 1);
  }
}

template <class T> 
void Node<T>::print() {
  tree_print_rec(*this, 0);
}

template class Node<std::string>;
