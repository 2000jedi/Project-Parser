#pragma once
#define TYPE_STK Node

typedef struct stack *stack;

extern stack new_stack(int size);

extern int stack_size(stack stk);

extern void stack_push(stack stk, TYPE_STK a);

extern TYPE_STK stack_pop(stack stk);

extern TYPE_STK stack_top(stack stk);

extern void stack_free(stack stk);

extern void stack_print(stack stk);