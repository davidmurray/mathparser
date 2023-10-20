//
//  queue.h
//  MathParser
//
//  Created by David Murray on 2014-09-22.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#ifndef MathParser_queue_h
#define MathParser_queue_h

#include <stdint.h>
#include "math_types.h"

typedef struct {
	int count;
	int last;
	int first;
	unsigned max;
	math_element *data;
} queue;

queue *queue_create(unsigned max);
void queue_free(queue *queue);

int queue_index(queue *queue);
int queue_empty(queue *stack);
math_element queue_top(queue *queue);
math_element queue_previous(queue *queue);

void queue_push(queue *queue, math_element item);
math_element queue_pop(queue *queue);

// For debugging
void queue_print(queue *queue);

#endif
