/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
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
	LedTask_t payload;

	while (true)
	{
		if (xSemaphoreTake(xSemaphore, 0) == pdPASS) {
			// Process the item from the priority queue

			taskENTER_CRITICAL();
			int queueSize = getQueueSize(&priorityQueue);
			taskEXIT_CRITICAL();
			LOGGER_INFO("current queue size: %d", queueSize);


			taskENTER_CRITICAL();
			if (!isQueueEmpty(&priorityQueue)) {

				item = dequeue(&priorityQueue);
				memcpy(&payload, item.data, sizeof(LedTask_t));
				vPortFree(item.data); // Free the allocated memory
			}
			taskEXIT_CRITICAL();

			if (payload.state == LED_CMD_ON)
			LOGGER_INFO("processing item with priority level: %d ", payload.priority);
			{

				switch (payload.color) {
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

void create_led_task( const char *task_name){
	if (xTaskCreate((TaskFunction_t)led_task_run, task_name, LED_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {

		LOGGER_INFO("Failed to create LED task");

	}


}


/* ============================================================================================ */
