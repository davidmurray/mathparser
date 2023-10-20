#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "stack.h"
#include "queue.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("not enough arguments.\n");
		return -1;
	}

	float result = 0.0f;

	math_error err = parse_expression((char *)argv[1], MATH_EXPRESSION_TYPE_INFIX, &result);
	if (err < 0) {
		fprintf(stderr, "Error: %d\n", err);
	} else {
		printf("%s = ", argv[1]);
		printf("%f\n", result);
	}

	return 0;
}

