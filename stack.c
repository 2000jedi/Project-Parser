#include "common.h"
#include "tree.h"

#define TYPE_STK Node

struct stack {
  TYPE_STK*arr;
  int head;
};

struct stack *new_stack(int size) {
  struct stack *stk = (struct stack *) malloc(sizeof(struct stack));
  stk->arr = calloc(sizeof(TYPE_STK), (size_t) size);
  stk->head = 0;
  return stk;
}

int stack_size(struct stack *stk) {
  return stk->head;
}

void stack_push(struct stack *stk, TYPE_STK a) {
  stk->arr[stk->head] = a;
  stk->head++;
}

TYPE_STK stack_pop(struct stack *stk) {
  return stk->arr[--stk->head];
}

TYPE_STK stack_top(struct stack *stk) {
  return stk->arr[(stk->head) - 1];
}

void stack_free(struct stack *stk) {
  free(stk->arr);
  free(stk);
}

void stack_print(struct stack *stk) {
  for (int i = 0; i < stk->head; ++i)  {
    printf("%s ", node_get_value(stk->arr[i]));
  }
  printf("\n");
}