#include "common.h"
#define TYPE_TREE char*

/**
 * A simple tree structure
 */
struct node {
  TYPE_TREE this;
  struct node* child[NODE_LEN];
  int num;
};

typedef struct node* Node;

Node new_node(TYPE_TREE x) {
  Node node = (Node)malloc(sizeof(struct node));
  node->this = x;
  node->num = 0;

  return node;
}

TYPE_TREE node_get_value(Node node) {
  return node->this;
}

void node_set_value(Node node, TYPE_TREE value) {
  free(node->this);
  node->this = value;
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
  printf("%s\n", node->this);

  for (i = 0; i < node->num; ++i) {
    tree_print_rec(node->child[i], indent + 1);
  }
}

void tree_free(Node n) {
  for (int i=0; i<n->num; ++i)
    tree_free(n->child[i]);
  free(n->this);
  free(n);
}

void PrintTree(Node tree) {
  tree_print_rec(tree, 0);
}
