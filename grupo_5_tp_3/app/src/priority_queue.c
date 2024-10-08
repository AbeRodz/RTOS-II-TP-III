/**
 * @file priority_queue.c
 * @brief This files implements the priority queue functions.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#include "logger.h"
#include "priority_queue.h"

/* ============================================================================================ */

void initQueue(PriorityQueue *q) 
{
    q->size = 0;
}

/* ============================================================================================ */

int isQueueEmpty(PriorityQueue *q) 
{
    return q->size == 0;
}

/* ============================================================================================ */

int isQueueFull(PriorityQueue *q) {
    return q->size == MAX_QUEUE_SIZE;
}

/* ============================================================================================ */

int getQueueSize(PriorityQueue *q) 
{
    return q->size;
}

/* ============================================================================================ */

void heapifyUp(PriorityQueue *q, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (q->items[index].priority > q->items[parentIndex].priority) {
            QueueItem_t temp = q->items[index];
            q->items[index] = q->items[parentIndex];
            q->items[parentIndex] = temp;
            index = parentIndex;
        } else {
            break;
        }
    }
}

/* ============================================================================================ */

void heapifyDown(PriorityQueue *q, int index) {
    while (1) {
        int largest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (leftChild < q->size && q->items[leftChild].priority > q->items[largest].priority) {
            largest = leftChild;
        }
        if (rightChild < q->size && q->items[rightChild].priority > q->items[largest].priority) {
            largest = rightChild;
        }
        if (largest != index) {
        	QueueItem_t temp = q->items[index];
            q->items[index] = q->items[largest];
            q->items[largest] = temp;
            index = largest;
        } else {
            break;
        }
    }
}

/* ============================================================================================ */

void enqueue(PriorityQueue *q, QueueItem_t item) {
    if (isQueueFull(q)) {

    	//LOGGER_INFO("queue is full...");
        return;
    }
    // Add the item to the end of the queue and heapify up. Then increment the size.
    q->items[q->size] = item;
    heapifyUp(q, q->size);
    q->size++;
}

/* ============================================================================================ */

QueueItem_t dequeue(PriorityQueue *q)
{
    if (isQueueEmpty(q))
    {
        // If the queue is empty, return an empty item

    	QueueItem_t empty = { .priority = LOW, .data = (void*)-1 };
        return empty;
    }
    // Assign the root to the last item in the queue (highest priority)
    QueueItem_t root = q->items[0];
    // Replace the last item and decrement the size. Then heapify down.
    q->items[0] = q->items[q->size - 1];
    q->size--;
    heapifyDown(q, 0);
    return root;
}

/* ============================================================================================ */
