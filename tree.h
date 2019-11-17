#pragma once

#define TYPE_TREE char*

typedef struct node* Node;

extern Node new_node(TYPE_TREE x);
extern TYPE_TREE node_get_value(Node node);
extern void node_set_value(Node node, TYPE_TREE val);
extern Node NodeChild(Node node, int i);
extern int NodeCnt(Node node);
extern void node_append_child(Node parent, Node child);
extern void node_append_tree(Node parent, Node root);
extern void PrintTree(Node tree);
extern void tree_free(Node n);
