//
//  stack.h
//  MathParser
//
//  Created by David Murray on 2014-09-22.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#ifndef MathParser_stack_h
#define MathParser_stack_h

#include <stdint.h>
#include "math_types.h"

typedef struct {
	int index;
	unsigned max;
	math_element *data;
} stack;

stack *stack_create(unsigned max);
void stack_free(stack *stack);

int stack_index(stack *stack);
int stack_empty(stack *stack);
math_element stack_top(stack *stack);
math_element stack_previous(stack *stack);

void stack_push(stack *stack, math_element item);
math_element stack_pop(stack *stack);

// For debugging
void stack_print(stack *stack);

#endif
