/*
 * Table Driver Parser
 */

#include "common.h"
#include "tree.h"
#include "stack.h"

#define MAX_RULE 500
#define MAX_RULE_LEN 100
#define BUFFER 255

int num_rule;
int start_rule;
char rule_table[MAX_RULE][STR_LEN];
char rules[MAX_RULE][MAX_RULE_LEN][STR_LEN];
int select[MAX_RULE][255];

void print_rules() {
  for (int i = 0; rules[i][0][0] != 0; ++i) {
    for (int j = 0; rules[i][j][0] != 0; ++j)
      printf("%s ", rules[i][j]);
    printf("\n");
  }
}

// find the integer representation of non-terminal
int find_rule(char *rule) {
  int i;
  for (i = 0; rule_table[i][0] != 0; ++i) {
    if (strcmp(rule_table[i], rule) == 0) {
      return i;
    }
  }
  return -1;
}

// find the length of the rule
int find_length(int rule_num) {
  int i;
  for (i = 0; rules[rule_num][i][0] != 0; ++i);
  return i;
}

// filter empty non-terminals (epsilons)
Node FilterEmpty(Node root) {
  if (node_get_value(root)[0] == '<' && NodeCnt(root) == 0)
    return NULL;

  Node tree = new_node(strdup(node_get_value(root)));

  for (int i = 0; i < NodeCnt(root); ++i) {
    Node fe = FilterEmpty(NodeChild(root, i));
    if (fe != NULL)
      node_append_child(tree, fe);
  }

  return tree;
}

// construct parse table from file
void TableConstructor(FILE *file) {
  memset(rule_table, 0, sizeof(rule_table));
  memset(rules, 0, sizeof(rules));
  for (int i = 0; i < MAX_RULE; ++i) {
    for (int j = 0; j < 255; ++j)
      select[i][j] = -1;
  }

  num_rule = 0;
  char start_symbol[BUFFER];

  char buf[BUFFER];

  // get start symbol

  if (fgets(buf, BUFFER, file) == NULL) {
    fprintf(stderr, "Failed to open parse table\n");
  }
  // remove trailing '\r\n'
  buf[strcspn(buf, "\r\n")] = 0;
  strcpy(start_symbol, buf);
  // get transition tables
  while (fgets(buf, BUFFER, file) != NULL) {
    buf[strcspn(buf, "\r\n")] = 0;
    // find rule for non-terminal
    char *s = strdup("");
    int j = 0;
    while (buf[j] != '=') {
      str_concat(&s, buf[j]);
      j++;
    }
    j++;
    strncpy(rules[num_rule][0], s, STR_LEN);
    free(s);
    int i = 1;
    while (j < strlen(buf)) {
      if (buf[j] == '<') {
        s = strdup("");
        while (buf[j] != '>') {
          str_concat(&s, buf[j]);
          j++;
        }
        str_concat(&s, '>');
        strncpy(rules[num_rule][i++], s, STR_LEN);
        free(s);
      } else {
        if (buf[j + 1] == '|') {
          s = char2str(buf[j]);
          str_concat(&s, buf[j + 1]);
          str_concat(&s, buf[j + 2]);
          j += 2;
          strncpy(rules[num_rule][i++], s, STR_LEN);
          free(s);
        } else {
          s = char2str(buf[j]);
          strncpy(rules[num_rule][i++], s, STR_LEN);
          free(s);
        }
      }
      j++;
    }

    // put the non-terminal into int table
    int cur_rule = -1;

    for (int k = 0; k < num_rule + 1; ++k) {
      if (rule_table[k][0] == 0) {
        strncpy(rule_table[k], rules[num_rule][0], STR_LEN);
        cur_rule = k;
        break;
      }
      if (strcmp(rules[num_rule][0], rule_table[k]) == 0) {
        cur_rule = k;
        break;
      }
    }

    if (rules[num_rule][1][0] == '<') {
      for (int k = 0; rule_table[k][0] != 0; ++k) {
        if (strcmp(rule_table[k], rules[num_rule][1]) == 0) {
          for (int q = 0; q < 255; ++q) {
            if (select[k][q] != -1) {
              select[cur_rule][q] = num_rule;
            }
          }
          break;
        }
      }
    } else {
      if (strlen(rules[num_rule][1]) == 3) {
        select[cur_rule][(int) rules[num_rule][1][0]] = num_rule;
        select[cur_rule][(int) rules[num_rule][1][2]] = num_rule;
      } else {
        select[cur_rule][(int) rules[num_rule][1][0]] = num_rule;
      }
    }

    num_rule++;
  }

  start_rule = find_rule(start_symbol);
  if (start_rule == -1) {
    fprintf(stderr, "Cannot find suitable start symbol: %s\n", start_symbol);
  }
}

Node PushdownAutomata(char *str) {
  Node tree = new_node(strdup(rule_table[start_rule]));
  stack stk = new_stack(MAX_RULE * MAX_RULE_LEN);
  stack_push(stk, tree);
  int p = 0; // the pointer to string
  while (str[p] == ' ') ++p;
  while (stack_size(stk) > 0) {
    Node cur_node = stack_pop(stk);
    if (node_get_value(cur_node)[0] == '<') { // non-terminal
      int cur_sym_int = find_rule(node_get_value(cur_node));
      if (select[cur_sym_int][(int) str[p]] != -1) { // the rule is found
        int cur_rul = select[cur_sym_int][(int) str[p]];
        for (int i = 1; i < find_length(cur_rul); ++i) {
          Node child = new_node(strdup(rules[cur_rul][i]));
          node_append_child(cur_node, child);
        }
        for (int i = NodeCnt(cur_node) - 1; i >= 0; --i) {
          stack_push(stk, NodeChild(cur_node, i));
        }
      } else { // does not match
        if (select[cur_sym_int]['~'] == -1) { // epsilon placeholder found
          fprintf(stderr, "Rejected at %s: %c\nCurrent string location: %s\n", node_get_value(cur_node), str[p], str + p);
          stack_free(stk);
          tree_free(tree);
          return NULL;
        }
      }
    } else {
      if (strlen(node_get_value(cur_node)) == 3) { // 'a|b' char a or char b
        if ((str[p] != node_get_value(cur_node)[0]) && (str[p] != node_get_value(cur_node)[2])) {
          fprintf(stderr, "Required %c or %c, found %c\n", node_get_value(cur_node)[0], node_get_value(cur_node)[2], str[p]);
          stack_free(stk);
          tree_free(tree);
          return NULL;
        } else {
          node_set_value(cur_node, char2str(str[p]));
        }
      } else {
        if (str[p] != node_get_value(cur_node)[0]) {
          fprintf(stderr, "Required %c, found %c\n", node_get_value(cur_node)[0], str[p]);
          stack_free(stk);
          tree_free(tree);
          return NULL;
        } else {
          node_set_value(cur_node, char2str(str[p]));
        }
      }
      ++p;
    }
  }
  if (stack_size(stk) > 0) {
    fprintf(stderr, "Required %c, EOF found\n", node_get_value(stack_top(stk))[0]);
    stack_free(stk);
    tree_free(tree);
    return NULL;
  }
  stack_free(stk);
  return tree;
}

Node parser(char *str, char *input) {
  FILE *file = fopen(str, "r");
  TableConstructor(file);

  Node PA = PushdownAutomata(input);
  if (PA == NULL) {
    fclose(file);
    return NULL;
  }
  Node FE = FilterEmpty(PA);
  tree_free(PA);
  fclose(file);
  return FE;
}
