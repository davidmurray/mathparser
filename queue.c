//
//  queue.c
//  MathParser
//
//  Created by David Murray on 2014-09-22.
//  Copyright (c) 2014 Cykey. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

queue *queue_create(unsigned max)
{
	if (max <= 0)
		return NULL;

	queue *aQueue = malloc(sizeof(queue));
	if (!aQueue)
		return NULL;

	math_element *data = calloc(max, sizeof(math_element));
	if (!data)
		return NULL;

	memset(aQueue, 0, sizeof(queue));
	aQueue->last = max - 1;
	aQueue->max = max;
	aQueue->data = data;

	return aQueue;
}

void queue_free(queue *queue)
{
	if (queue != NULL) {
		free(queue->data);
		free(queue);
	}
}

int queue_empty(queue *queue)
{
	return (queue->count == 0);
}

int queue_index(queue *queue)
{
	if (!queue) {
		return 0;
	}

	return queue->count;
}

math_element queue_top(queue *queue)
{
	if (!queue) {
		return EMPTY_TOKEN;
	}

	if (queue->count == 0) {
		fprintf(stderr, "Error: queue is empty\n");
		return EMPTY_TOKEN;
	}

	return queue->data[queue->last];
}

math_element queue_previous(queue *queue)
{
	if (!queue) {
		return EMPTY_TOKEN;
	}

	if (queue->count == 0) {
		fprintf(stderr, "Error: queue is empty\n");
		return EMPTY_TOKEN;
	}

	return queue->data[queue->last-1];
}

void queue_push(queue *queue, math_element item)
{
	if (!queue) {
		return;
	}

	if (queue->count >= queue->max)
		fprintf(stderr, "Error: queue is full\n");
	else {
		queue->last = (queue->last + 1) % queue->max;
		queue->data[queue->last] = item;
		queue->count++;
	}
}

math_element queue_pop(queue *queue)
{
	if (!queue) {
		return EMPTY_TOKEN;
	}

	math_element data = {0, {0}};

	if (queue->count <= 0) {
		fprintf(stderr, "Error: queue is empty\n");
	} else {
		data = queue->data[queue->first];
		queue->first = (queue->first + 1) % queue->max;
		queue->count--;
	}

	return data;
}

void queue_print(queue *queue)
{
	if (!queue) {
		return;
	}

	printf("=== QUEUE ===\n");

	for (int i = 0; i < queue->count; i++) {
		math_element data = queue->data[i];
		if (data.type != 0) {
			if (data.type == TOKEN_TYPE_PARENTHESIS || data.type == TOKEN_TYPE_OPERATOR)
				printf("\t[%d] %d, %c\n", i, data.type, data.value.char_value);
			else if (data.type == TOKEN_TYPE_NUMBER)
				printf("\t[%d] %d, %f\n", i, data.type, data.value.float_value);
		}
	}

	printf("=============\n\n");
}
