/*
 * Table Driver Parser
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include "tree.hpp"
#include "parser.hpp"

#define MAX_RULE 500
#define MAX_RULE_LEN 100
#define BUFFER 255

int start_rule;
std::vector<std::string> rule_table;
std::vector<catagory> rules;
int selection[MAX_RULE][255];

void print_rules() {
  /** debug use only
   * print_rules - print all catagory rules out
   */
  #ifdef DEBUG
  for (unsigned int i = 0; i < rules.size(); ++i) {
    std::cout << rules[i].name << " ";
    for (unsigned int j = 0; j < rules[i].reps.size() ; ++j)
      std::cout << rules[i].reps[j] << ' ';
    std::cout << std::endl;
  }
  #endif
}

int find_rule(std::string rule) {
  /**
   * find_rule - find the integer representation of non-terminal
   * 
   * @inputs
   * rule - the destiny rule to find
   */
  int i;
  for (i = 0; rule_table[i][0] != 0; ++i) {
    if (rule_table[i] == rule) {
      return i;
    }
  }
  return -1;
}

// filter empty non-terminals (epsilons)
Node FilterEmpty(Node root) {
  if (node_get_value(root)[0] == '<' && NodeCnt(root) == 0)
    return NULL;

  Node tree = new_node(node_get_value(root));

  for (int i = 0; i < NodeCnt(root); ++i) {
    Node fe = FilterEmpty(NodeChild(root, i));
    if (fe != NULL)
      node_append_child(tree, fe);
  }

  return tree;
}

// construct parse table from file
void TableConstructor(std::ifstream *fd) {
  rules.clear();
  rule_table.clear();
  for (int i = 0; i < MAX_RULE; ++i) {
    for (int j = 0; j < 255; ++j)
      selection[i][j] = -1;
  }

  std::string start_symbol;
  std::string buf;
  // get start symbol
  if (! std::getline(*fd, start_symbol)) {
    std::cerr << "Failed to open parse table" << std::endl;
  }

  // get transition tables
  while (std::getline(*fd, buf)) {
    // find rule for non-terminal

    // get substring for catagory name
    std::string s = buf.substr(0, buf.find("="));
    rules.push_back(catagory(s));
    int j = buf.find("=") + 1;

    // get each identifier
    while (j < (int) buf.length()) {
      if (buf[j] == '<') {
        s = "";
        while (buf[j] != '>') {
          s += buf[j];
          j++;
        }
        s += '>';
        rules.back().reps.push_back(s);
      } else {
        s = "";
        while ((j < (int) buf.size() - 2) && (buf[j+1] == '|')) { // concats a|b
          s += buf[j];
          j += 2;
        }
        s += buf[j];
        rules.back().reps.push_back(s);
      }
      j++;
    }

    // put the non-terminal into rule table
    int cur_rule = -1;
    std::vector<std::string>::iterator it = std::find(rule_table.begin(), rule_table.end(), rules.back().name);
    if (it == rule_table.end()) {
      // rule not found
      rule_table.push_back(rules.back().name);
      cur_rule = rule_table.size() - 1;
    } else {
      cur_rule = std::distance(rule_table.begin(), it);
    }

    // set the selection table
    if (rules.back().reps[0][0] == '<') {
      for (int k = 0; rule_table[k][0] != 0; ++k) {
        if (rule_table[k] == rules.back().reps[0]) {
          for (int q = 0; q < 255; ++q) {
            if (selection[k][q] != -1) {
              selection[cur_rule][q] = rules.size() - 1;
            }
          }
          break;
        }
      }
    } else {
      for (auto c : rules.back().reps[0]) { // terminals
        selection[cur_rule][(int) c] = rules.size() - 1;
      }
    }
  }

  start_rule = find_rule(start_symbol);
  if (start_rule == -1) {
    std::cerr << "Cannot find suitable start symbol: " << start_symbol << std::endl;
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
      if (selection[cur_sym_int][(int) str[p]] != -1) { // the rule is found
        int cur_rul = selection[cur_sym_int][(int) str[p]];
        for (int i = 1; i < find_length(cur_rul); ++i) {
          Node child = new_node(strdup(rules[cur_rul][i]));
          node_append_child(cur_node, child);
        }
        for (int i = NodeCnt(cur_node) - 1; i >= 0; --i) {
          stack_push(stk, NodeChild(cur_node, i));
        }
      } else { // does not match
        if (selection[cur_sym_int]['~'] == -1) { // epsilon placeholder found
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


Node parser(std::string parse_table, std::string input) {
  std::ifstream fp(parse_table);
  TableConstructor(&fp);
  print_rules();

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
