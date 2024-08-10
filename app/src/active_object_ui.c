/**
 * @file active_object_ui.c
 * @brief This file implements the UI active object.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "task_led.h"
#include "active_object_ui.h"
#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "app.h"
#include "priority_queue.h"

/* ============================================================================================ */

/* UI events queue */
QueueHandle_t ui_event_queue;

/* UI task */
PriorityQueue priorityQueue;

/* ============================================================================================ */

void ui_task_init(UiTask_t *ui_task, QueueHandle_t button_state_queue, LedTask_t *led_task)
{
	ui_task->button_state_queue = button_state_queue;
	ui_task->led_task = led_task;
}

/* ============================================================================================ */

void ui_task_run(void *argument)
{
	UiTask_t* ui_task = (UiTask_t*)argument;
	message_t message;
	LedTask_t payload;
	QueueItem_t item;

	/* Run the UI task */
	while (true)
	{
		/* Wait for a button state change */
		if (xQueueReceive(ui_task->button_state_queue, &message, portMAX_DELAY) == pdPASS)
		{
			/* Process the button state according to its message assigned on task_button() */
			switch (message.button)
			{
				case BUTTON_STATE_PULSE:
					LOGGER_INFO("Button pulse detected");
					// Fill the payload with the necessary information
					payload.color = LED_COLOR_RED;
					payload.state = LED_CMD_ON;
					payload.name = "RED LED Task";
					payload.priority = HIGH;
					// Assign the payload to the queue item
					item.priority = payload.priority;
					item.data = pvPortMalloc(sizeof(LedTask_t));
					memcpy(item.data, &payload, sizeof(LedTask_t));
					// Add the item to the priority queue
					taskENTER_CRITICAL();
					enqueue(&priorityQueue, item);
					taskEXIT_CRITICAL();
					// Give the semaphore to the scheduler
					xSemaphoreGive(xSemaphore);
					break;


				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));
				enqueue_semaphore(&priorityQueue, item);

				break;
			case BUTTON_STATE_SHORT:
				LOGGER_INFO("Button short press detected");
				payload.color = LED_COLOR_GREEN;
				payload.state = LED_CMD_ON;
				payload.name = "Green LED Task";
				payload.priority = MEDIUM;

				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));
				enqueue_semaphore(&priorityQueue, item);

				break;
			case BUTTON_STATE_LONG:
				LOGGER_INFO("Button long press detected");
				payload.color = LED_COLOR_BLUE;
				payload.state = LED_CMD_ON;
				payload.name = "Blue LED Task";
				payload.priority = LOW;

				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));

				enqueue_semaphore(&priorityQueue, item);


				break;
			default:
				break;
			}

		}
	}
}


void enqueue_semaphore(PriorityQueue *q, QueueItem item){
	taskENTER_CRITICAL();
	enqueue(q, item);
	taskEXIT_CRITICAL();
	xSemaphoreGive(xSemaphore);
}

/* ============================================================================================ */

void ui_send_message(message_t *pmsg)
{
	xQueueSend(ui_event_queue,pmsg	,	pdMS_TO_TICKS(10) );
}

/* ============================================================================================ */

void ui_task_create(UiTask_t *ui_task) 
{
	/* Create the UI event queue */
	ui_event_queue = xQueueCreate(10, sizeof(message_t));

	/* Initialize the UI task */
	ui_task_init(ui_task, ui_event_queue, &led_task);

	/* Create the UI task */
	BaseType_t status;

	status = xTaskCreate(ui_task_run, "UI Task", configMINIMAL_STACK_SIZE, ui_task, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}

/* ============================================================================================ */