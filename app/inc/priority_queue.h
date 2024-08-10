/**
 * @file priority_queue.h
 * @brief This files declares the priority queue functions.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#ifndef INC_PRIORITY_QUEUE_H_
#define INC_PRIORITY_QUEUE_H_

/* ============================================================================================ */

#include <stdio.h>
#include <stdlib.h>

/* ============================================================================================ */

#define MAX_HEAP_SIZE 10

/**
 * @brief This enum defines the priority levels 
 */
typedef enum {
    LOW,
    MEDIUM,
    HIGH
} Priority;

/**
 * @brief This struct defines the queue item
 */
typedef struct {
    Priority priority;
    void* data; // Replace with any data type as needed
}QueueItem_t;

/* ============================================================================================ */

/**
 * @brief This struct defines the priority queue
 */
typedef struct {
    QueueItem_t items[MAX_HEAP_SIZE];
    int size;
} PriorityQueue;

/* ============================================================================================ */

/**
 * @brief This function initializes the queue
 * @param q This is a pointer to the priority queue
 */
void initQueue(PriorityQueue *q);

/**
 * @brief This function checks if the queue is empty
 * @param q This is a pointer to the priority queue
 * @return 1 if the queue is empty, 0 otherwise
 */
int isQueueEmpty(PriorityQueue *q);

/* ============================================================================================ */

/**
 * @brief This function checks if the queue is full
 * @param q This is a pointer to the priority queue
 * @return 1 if the queue is full, 0 otherwise
 */
int isQueueFull(PriorityQueue *q);

/* ============================================================================================ */

/**
 * @brief This function gets the size of the queue
 * @param q This is a pointer to the priority queue
 * @return The size of the queue
 */
int getQueueSize(PriorityQueue *q);

/* ============================================================================================ */

/**
 * @brief This function heapifies up the queue. It is used when a new item is added to the queue to maintain the heap priority order.
 * @param q This is a pointer to the priority queue
 * @param index This is the index to heapify up
 */
void heapifyUp(PriorityQueue *q, int index);

/* ============================================================================================ */

/**
 * @brief This function heapifies down the queue. It is used when an item is removed from the queue to maintain the heap priority order.
 * @param q This is a pointer to the priority queue
 * @param index This is the index to heapify down
 */
void heapifyDown(PriorityQueue *q, int index);

/* ============================================================================================ */

/**
 * @brief This function enqueues an item to the queue
 * @param q This is a pointer to the priority queue
 * @param item This is the item to enqueue
 */
void enqueue(PriorityQueue *q, QueueItem_t item);

/* ============================================================================================ */

/**
 * @brief This function dequeues an item from the queue
 * @param q This is a pointer to the priority queue
 * @return The dequeued item
 */
QueueItem_t dequeue(PriorityQueue *q);

/* ============================================================================================ */

#endif /* INC_PRIORITY_QUEUE_H_ */