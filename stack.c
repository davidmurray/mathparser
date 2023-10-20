//
//  stack.c
//  MathParser
//
//  Created by David Murray on 2014-09-22.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"

stack *stack_create(unsigned max)
{
	if (max <= 0)
		return NULL;

	stack *aStack = calloc(1, sizeof(stack));
	if (!aStack)
		return NULL;

	math_element *data = calloc(max, sizeof(math_element));
	if (!data)
		return NULL;

	memset(aStack, 0, sizeof(stack));

	aStack->max = max;
	aStack->data = data;

	return aStack;
}

void stack_free(stack *stack)
{
	if (stack != NULL) {
		free(stack->data);
		free(stack);
	}
}

int stack_index(stack *stack)
{
	if (!stack) {
		return 0;
	}

	return stack->index;
}

int stack_empty(stack *stack)
{
	if (!stack) {
		return 0;
	}

	return (stack->index == 0);
}

math_element stack_top(stack *stack)
{
	if (!stack) {
		return EMPTY_TOKEN;
	}

	if (stack->index == 0) {
		return EMPTY_TOKEN;
	}

	return stack->data[stack->index];
}

math_element stack_previous(stack *stack)
{
	if (!stack) {
		return EMPTY_TOKEN;
	}

	if (stack->index == 0) {
		return EMPTY_TOKEN;
	}

	return stack->data[stack->index-1];
}

void stack_push(stack *stack, math_element item)
{
	if (!stack) {
		return;
	}

	if (stack->index < stack->max) {
		stack->index++;
		stack->data[stack->index] = item;
	} else {
		fprintf(stderr, "Error: stack is full\n");
	}
}

math_element stack_pop(stack *stack)
{
	if (!stack) {
		return EMPTY_TOKEN;
	}

	math_element data = {0, {0}};

	if (stack->index == 0) {
		fprintf(stderr, "Error: stack is empty\n");
	} else {
		// Keep a backup so we can use it later.
		data = stack->data[stack->index];

		stack->data[stack->index] = EMPTY_TOKEN;
		stack->index--;
	}

	return data;
}

void stack_print(stack *stack)
{
	if (!stack) {
		return;
	}

	printf("=== STACK ===\n");

	for (int i = stack_index(stack); i > 0; i--) {
		math_element data = stack->data[i];
		if (data.type != 0) {
			if (data.type == TOKEN_TYPE_PARENTHESIS || data.type == TOKEN_TYPE_OPERATOR)
				printf("\t[%d] %d, %c\n", i, data.type, data.value.char_value);
			else if (data.type == TOKEN_TYPE_NUMBER)
				printf("\t[%d] %d, %f\n", i, data.type, data.value.float_value);
		}
	}

	printf("=============\n");
}
