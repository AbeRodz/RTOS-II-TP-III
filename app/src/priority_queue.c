/*
 * priority_queue.c
 *
 *  Created on: Aug 7, 2024
 *      Author: rodz
 */

#include "logger.h"
#include "priority_queue.h"


void initQueue(PriorityQueue *q) {
    q->size = 0;
}

int isQueueEmpty(PriorityQueue *q) {
    return q->size == 0;
}

int isQueueFull(PriorityQueue *q) {
    return q->size == MAX_HEAP_SIZE;
}

int getQueueSize(PriorityQueue *q) {
    return q->size;
}

void heapifyUp(PriorityQueue *q, int index) {
    int parentIndex = (index - 1) / 2;
    if (index && q->items[index].priority > q->items[parentIndex].priority) {
        QueueItem temp = q->items[index];
        q->items[index] = q->items[parentIndex];
        q->items[parentIndex] = temp;
        heapifyUp(q, parentIndex);
    }
}

void heapifyDown(PriorityQueue *q, int index) {
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
        QueueItem temp = q->items[index];
        q->items[index] = q->items[largest];
        q->items[largest] = temp;
        heapifyDown(q, largest);
    }
}

void enqueue(PriorityQueue *q, QueueItem item) {
    if (isQueueFull(q)) {

    	//LOGGER_INFO("queue is full...");
        return;
    }
    q->items[q->size] = item;
    heapifyUp(q, q->size);
    q->size++;
}

QueueItem dequeue(PriorityQueue *q) {
    if (isQueueEmpty(q)) {
    	//LOGGER_INFO("queue is empty...");
        QueueItem empty = { .priority = LOW, .data = -1 }; // Return an empty item
        return empty;
    }
    QueueItem root = q->items[0];
    q->items[0] = q->items[q->size - 1];
    q->size--;
    heapifyDown(q, 0);
    return root;
}
