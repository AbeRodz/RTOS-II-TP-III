/**
 * @file active_object_ui.h
 * @brief This file declares the UI active object.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#ifndef INC_ACTIVE_OBJECT_UI_H
#define INC_ACTIVE_OBJECT_UI_H

/* ============================================================================================ */

#include "main.h"
#include "cmsis_os.h"
#include "active_object_led.h"

/* ============================================================================================ */

/* This enum defines the possible states of the button */
typedef enum
{
    BUTTON_STATE_NONE,
    BUTTON_STATE_PULSE,
    BUTTON_STATE_SHORT,
    BUTTON_STATE_LONG
} button_state_t;

/* This struct defines the UI task */
typedef struct
{
    QueueHandle_t button_state_queue; // Queue to receive button states
    LedTask_t *led_task; // LED task
} UiTask_t;

/* UI events queue */
extern QueueHandle_t ui_event_queue;

/* UI task */
extern UiTask_t ui_task;

/* ============================================================================================ */

/**
 * @brief This function creates the UI task
 * @param ui_task This is a pointer to the UI task
 */
void ui_task_create(UiTask_t *ui_task);

/* ============================================================================================ */

/**
 * @brief This function sends a message to ui queue.
 * @param pmsg  pointer with message
 */
void ui_send_message(message_t *pmsg);

/* ============================================================================================ */

/**
 * @brief This function initializes the UI task
 * @param ui_task This is a pointer to the UI task
 * @param button_state_queue This is the queue to receive button states
 * @param red_task This is a pointer to the red LED task
 * @param green_task This is a pointer to the green LED task
 * @param blue_task This is a pointer to the blue LED task
 */
void ui_task_init(UiTask_t *ui_task, QueueHandle_t button_state_queue, LedTask_t *led_task);

/* ============================================================================================ */

/**
 * @brief This function runs the UI task
 * @param argument This is a pointer to the UI task
 */
void ui_task_run(void *argument);

/* ============================================================================================ */

/**
 * @brief This function enqueues an item in the priority queue and gives the semaphore
 * @param q This is a pointer to the priority queue
 * @param item This is the item to be enqueued
 */
void enqueue_semaphore(PriorityQueue *q, QueueItem_t item);

/* ============================================================================================ */

#endif /* INC_ACTIVE_OBJECT_UI_H */
