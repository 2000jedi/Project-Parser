#include <string>
#include <iostream>

#define TYPE_TREE std::string
#define NODE_LEN 100

/**
 * A simple tree structure
 */
struct node {
  TYPE_TREE t;
  struct node* child[NODE_LEN];
  int num;
};

typedef struct node* Node;

Node new_node(TYPE_TREE x) {
  Node node = (Node)malloc(sizeof(struct node));
  node->t = x;
  node->num = 0;

  return node;
}

TYPE_TREE node_get_value(Node node) {
  return node->t;
}

void node_set_value(Node node, TYPE_TREE value) {
  node->t = value;
}

void node_append_child(Node parent, Node child) {
  parent->child[parent->num++] = child;
}

Node NodeChild(Node node, int i) {
  return node->child[i];
}

int NodeCnt(Node node) {
  return node->num;
}

void node_append_tree(Node parent, Node root) {
  parent->child[parent->num++] = root;
}

void tree_print_rec(Node node, int indent) {
  int i;
  for (i = 0; i < indent; ++i) printf("  ");
  std::cout << node->t << std::endl;

  for (i = 0; i < node->num; ++i) {
    tree_print_rec(node->child[i], indent + 1);
  }
}

void tree_free(Node n) {
  for (int i=0; i<n->num; ++i)
    tree_free(n->child[i]);
  free(n);
}

void PrintTree(Node tree) {
  tree_print_rec(tree, 0);
}
