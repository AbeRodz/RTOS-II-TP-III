/**
 * @file active_object_led.c
 * @brief This file implements the LED active object.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "app.h"
#include "active_object_led.h"
#include "task_led.h"
#include "task_button.h"
#include "priority_queue.h"

/* ============================================================================================ */

void led_task_run(void)
{
	QueueItem item; 
	LedTask_t payload; // Payload with resources for the LED task

	while (true)
	{
		// Wait for the semaphore to be available
		if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdPASS) {
			// Process the item from the priority queue

			// Get the queue size
			taskENTER_CRITICAL();
			int queueSize = getQueueSize(&priorityQueue);
			taskEXIT_CRITICAL();

			// If there are 5 or more items in the queue
			if (queueSize > 5) 
			{
				// Process the item from the priority queue
				taskENTER_CRITICAL();
				if (!isQueueEmpty(&priorityQueue)) 
				{
					// If queue is not empty, dequeue the item
					item = dequeue(&priorityQueue);
					memcpy(&payload, item.data, sizeof(LedTask_t));
					vPortFree(item.data); // Free the allocated memory
				}
				taskEXIT_CRITICAL();

				// Process the item. Turn on the corresp. LED for 5 seconds and then turn it off.
				if (payload.state == LED_CMD_ON)
				{
					switch (payload.color) 
					{
						case LED_COLOR_RED:
							led_red_set_state(payload.state);
							vTaskDelay(pdMS_TO_TICKS(5000));
							led_red_set_state(LED_CMD_OFF);
							break;
						case LED_COLOR_GREEN:
							led_green_set_state(payload.state);
							vTaskDelay(pdMS_TO_TICKS(5000));
							led_green_set_state(LED_CMD_OFF);
							break;
						case LED_COLOR_BLUE:
							led_blue_set_state(payload.state);
							vTaskDelay(pdMS_TO_TICKS(5000));
							led_blue_set_state(LED_CMD_OFF);
							break;
						default:
							break;
					}
				}
			}
		}
	}
}

/* ============================================================================================ */

void led_red_set_state(led_cmd_t cmd) 
{
	HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ============================================================================================ */

void led_green_set_state(led_cmd_t cmd) 
{
	HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ============================================================================================ */

void led_blue_set_state(led_cmd_t cmd) 
{
	HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ============================================================================================ */

void create_led_task( const char *task_name)
{
	if (xTaskCreate((TaskFunction_t)led_task_run, task_name, LED_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) 
	{

		LOGGER_INFO("Failed to create LED task");
	}
}

/* ============================================================================================ */
