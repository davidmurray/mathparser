//
//  parser.c
//  MathParser
//
//  Created by David Murray on 2014-09-21.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include "parser.h"

// Private functions
static math_error convert_to_postfix(queue *tokens, queue **outQueue); // You are responsible for freeing queue **outQueue.
static math_error tokenize(char *expression, queue **outQueue); // You are responsible for freeing queue **outQueue.
static math_error evalutate_expression(queue *tokens, float *result);
static int is_operator(char token);
static int operator_precendence(char operator);
static operator_associativity operator_associativity_f(char operator);

math_error parse_expression(char *expression, expression_type type, float *result)
{
	queue *tokens = NULL;
	math_error ret = 0;

	//math_element *tokens = NULL;
	ret = tokenize(expression, &tokens);
	if (ret < 0) {
		goto out;
	}

	// First, convert it to postfix.
	if (type == MATH_EXPRESSION_TYPE_INFIX) {
		// TODO: Handle error.
		queue *postfixTokens = NULL;
		ret = convert_to_postfix(tokens, &postfixTokens);
		if (ret < 0) {
			printf("leaving..\n");
			goto out;
		} else {
			queue_free(tokens);
			tokens = postfixTokens;
		}
	}

	ret = evalutate_expression(tokens, result);

out:
	queue_free(tokens);

	return ret;
}

// Beware, '(' and ')' are not operators!
static int is_operator(char token)
{
	if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^')
		return 1;
	else
		return 0;
}

static int operator_precendence(char operator)
{
	switch (operator) {
		case '^':
			return 4;
		case '*':
		case '/':
			return 3;
		case '+':
		case '-':
			return 2;
		default:
			return 0;
	}

	return 0;
}

static operator_associativity operator_associativity_f(char operator)
{
	if (operator == '+' || operator == '-' || operator == '*' || operator == '/')
		return OPERATOR_ASSOCIATIVITY_LEFT;
	else
		return OPERATOR_ASSOCIATIVITY_RIGHT;

}

static math_error tokenize(char *expression, queue **outQueue) // You are responsible for freeing queue **outQueue.
{
	math_error ret = 0;

	*outQueue = queue_create(32);
	if (!*outQueue) {
		ret = ERROR_NOT_ENOUGH_MEMORY;
		goto out;
	}

	char *p = expression;

	while (p[0]) {
		printf("char: %s\n", p);

		if (is_operator(p[0])) {
			math_element element = {TOKEN_TYPE_OPERATOR, {0}};
			element.value.char_value = p[0];
			queue_push(*outQueue, element);

			p++;
		}  else if (isdigit(p[0])) {
			errno = 0;
			// Cast to float because strtof does not exist in avr-libc.
			float value = (float)strtod(p, &p); // TODO: more error checking.
			if (value == 0 && errno != 0) {
				//fprintf(stderr, "Something went wrong with strtod()! %s\n", strerror(errno));

				ret = ERROR_COULD_NOT_PARSE_NUMBER;
				goto out;
			}

			math_element element = {TOKEN_TYPE_NUMBER, {0}};
			element.value.float_value = value;
			queue_push(*outQueue, element);
		} else if (p[0] == '(' || p[0] == ')') {
			//if (p[0] == '(' && p[-1] == ')') {
			//	math_element multiplication = {TOKEN_TYPE_OPERATOR, {0}};
			//	multiplication.value.char_value = '*';
			//	queue_push(*outQueue, multiplication);
			//}
			math_element element = {TOKEN_TYPE_PARENTHESIS, {0}};
			element.value.char_value = p[0];

			queue_push(*outQueue, element);

			p++;
		} else if (p[0] == ' ') {
			p++;
		} else {
			ret = ERROR_UNRECOGNIZED_CHARACTER;
			goto out;
		}
	}

out:
	return ret;
}

static math_error evalutate_expression(queue *tokens, float *result)
{
	#ifndef __AVR && DEBUG // wrong
	//fprintf(stderr, "evalutate_expression\n");
	//queue_print(tokens);
	#endif

	math_error ret = 0;

	if (!tokens) {
		ret = ERROR_WRONG_PARAMETERS;
		goto out;
	}

	stack *outputStack = stack_create(32);
	if (!outputStack) {
		ret = ERROR_NOT_ENOUGH_MEMORY;
		goto out;
	}

	int i = 0;
	for (i = 0; i < tokens->count; i++) {
		math_element token = tokens->data[i];

		if (token.type == TOKEN_TYPE_NUMBER) {
			stack_push(outputStack, token);
		} else if (token.type == TOKEN_TYPE_OPERATOR) {
			// We need at least two numbers for this. If we have less, there's a problem.
			if (stack_index(outputStack) < 2) {
				ret = ERROR_NOT_ENOUGH_OPERANDS;
				goto out;
			}

			float first = stack_pop(outputStack).value.float_value;
			float second = stack_pop(outputStack).value.float_value;

			//fprintf(stderr, "operator: %c\n", token.value.char_value);

			math_element element = {TOKEN_TYPE_NUMBER, {0}};

			if (token.value.char_value == '+') {
				element.value.float_value = second + first;
				stack_push(outputStack, element);
			} else if (token.value.char_value  == '-') {
				element.value.float_value = second - first;
				stack_push(outputStack, element);
			} else if (token.value.char_value  == '*') {
				element.value.float_value = second * first;
				stack_push(outputStack, element);
			} else if (token.value.char_value  == '/') {
				element.value.float_value = second / first;
				stack_push(outputStack, element);
			}  else if (token.value.char_value  == '^') {
				element.value.float_value = pow(second, first);
				stack_push(outputStack, element);
			}
		}
		//stack_print(outputStack);
	}

	// We need one, and *only* one element in the stack at this point.
	if (stack_index(outputStack) != 1) {
		ret = ERROR_TOO_MANY_OPERANDS;
		goto out;
	}

	float top = stack_top(outputStack).value.float_value; // TODO: check for error here
	*result = top;

	printf("top: %f, %f\n", top, *result);

out:
	stack_free(outputStack);

	return ret;
}

static math_error convert_to_postfix(queue *tokens, queue **outQueue)
{
	math_error ret = 0;

	*outQueue = queue_create(32);
	if (!outQueue) {
		ret = ERROR_NOT_ENOUGH_MEMORY;
		goto out;
	}

	stack *operatorsStack = stack_create(32);
	if (!operatorsStack) {
		ret = ERROR_NOT_ENOUGH_MEMORY;
		goto out;
	}

	for (int i = 0; i < tokens->count; i++) {
		math_element token = tokens->data[i];

		switch (token.type) {
			case TOKEN_TYPE_NUMBER:
				queue_push(*outQueue, token);
				break;
			case TOKEN_TYPE_OPERATOR: {
				int tokenPrecendence = operator_precendence(token.value.char_value);

				math_element top = stack_top(operatorsStack);
				while (operator_precendence(top.value.char_value) >= tokenPrecendence && operator_associativity_f(token.value.char_value) == OPERATOR_ASSOCIATIVITY_LEFT) { // THIS IS NOT RIGHT, I THINK. MUST FIX
					stack_pop(operatorsStack);
					queue_push(*outQueue, top);

					if (stack_empty(operatorsStack)) {
						break;
					}
				}

				stack_push(operatorsStack, token);
				break;
			} case TOKEN_TYPE_PARENTHESIS: {
				if (token.value.char_value == '(') {
					// Simply push it to the stack.
					stack_push(operatorsStack, token);
				} else if (token.value.char_value == ')') {
					for (int i = stack_index(operatorsStack); i >= 0; i--) {
						math_element data = operatorsStack->data[i];

						if (data.value.char_value != '(') {
							stack_pop(operatorsStack);
							queue_push(*outQueue, data);
						} else { // TODO: Some more error checking here.
							// We found the left parenthesis, pop it off the stack but NOT onto the output queue.
							stack_pop(operatorsStack);

							break;
						}

						printf("i: %d\n", i);
					}
				}

				break;
			} default: // Handle unrecognized
				break;
		}
	}

	while (stack_top(operatorsStack).type != 0) {
		math_element element = stack_top(operatorsStack);

		if (element.type == TOKEN_TYPE_PARENTHESIS && element.value.char_value == '(') {
			ret = ERROR_OPEN_PARENTHESIS;
			goto out;
		}

		stack_pop(operatorsStack);
		queue_push(*outQueue, element);
	}

out:
	stack_free(operatorsStack);

	// check if ret < 0, if so return queue null
	if (ret < 0) {
		queue_free(*outQueue);
		*outQueue = NULL;
	}

	return ret;
}
