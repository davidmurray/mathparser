//
//  parser.h
//  MathParser
//
//  Created by David Murray on 2014-09-21.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#ifndef MathParser_parser_h
#define MathParser_parser_h

#include <stdint.h>
#include "stack.h"
#include "queue.h"
#include "math_types.h"

math_error parse_expression(char *expression, expression_type type, float *result);

#endif
