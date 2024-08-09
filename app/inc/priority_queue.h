/*
 * priority_queue.h
 *
 *  Created on: Aug 7, 2024
 *      Author: rodz
 */

#ifndef INC_PRIORITY_QUEUE_H_
#define INC_PRIORITY_QUEUE_H_


#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 10

typedef enum {
    LOW,
    MEDIUM,
    HIGH
} Priority;

typedef struct {
    Priority priority;
    void* data; // Replace with any data type as needed
} QueueItem;

typedef struct {
    QueueItem items[MAX_HEAP_SIZE];
    int size;
} PriorityQueue;

// Function prototypes
void initQueue(PriorityQueue *q);
int isQueueEmpty(PriorityQueue *q);
int isQueueFull(PriorityQueue *q);
void enqueue(PriorityQueue *q, QueueItem item);
QueueItem dequeue(PriorityQueue *q);
void heapifyUp(PriorityQueue *q, int index);
void heapifyDown(PriorityQueue *q, int index);
int getQueueSize(PriorityQueue *q);

#endif /* INC_PRIORITY_QUEUE_H_ */
