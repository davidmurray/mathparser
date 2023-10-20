//
//  math_types.h
//  MathParser
//
//  Created by David Murray on 2014-09-30.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#ifndef MathParser_math_types_h
#define MathParser_math_types_h

typedef union {
	char char_value;
	float float_value;
} math_value;

typedef struct {
	uint8_t type;
	math_value value;
} math_element;

typedef enum {
	TOKEN_TYPE_OPERATOR = 1,
	TOKEN_TYPE_PARENTHESIS = 2,
	TOKEN_TYPE_NUMBER = 3
} token_type;

typedef enum {
	MATH_EXPRESSION_TYPE_INFIX = 0,
	MATH_EXPRESSION_TYPE_POSTFIX = 1
} expression_type;

typedef enum {
	OPERATOR_ASSOCIATIVITY_LEFT = 0,
	OPERATOR_ASSOCIATIVITY_RIGHT = 1
} operator_associativity;

typedef enum {
	ERROR_UNRECOGNIZED_CHARACTER = -11,
	ERROR_NOT_ENOUGH_MEMORY = -10,
	ERROR_NOT_ENOUGH_OPERANDS = -9,
	ERROR_TOO_MANY_OPERANDS = -8,
	ERROR_NOT_ENOUGH_OPERATORS = -7,
	ERROR_TOO_MANY_OPERATORS = -6,
	ERROR_OPEN_PARENTHESIS = -5,
	ERROR_CLOSE_PARENTHESIS = -4,
	ERROR_COULD_NOT_PARSE_NUMBER = -3,
	ERROR_WRONG_PARAMETERS = -2
} math_error;

static const math_element EMPTY_TOKEN = {0, {0}};

#endif
