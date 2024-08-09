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


QueueHandle_t ui_event_queue;
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
	QueueItem item;
	while (true)
	{
		/* Wait for a button state change */
		if (xQueueReceive(ui_task->button_state_queue, &message, portMAX_DELAY) == pdPASS)
		{

			switch (message.button)
			{
			case BUTTON_STATE_PULSE:
				LOGGER_INFO("Button pulse detected");
				payload.color = LED_COLOR_RED;
				payload.state = LED_CMD_ON;
				payload.name = "RED LED Task";
				payload.priority = HIGH;

				//create_led_task(payload);
				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));
				taskENTER_CRITICAL();
				enqueue(&priorityQueue, item);
				taskEXIT_CRITICAL();
				xSemaphoreGive(xSemaphore);

				break;
			case BUTTON_STATE_SHORT:
				LOGGER_INFO("Button short press detected");
				payload.color = LED_COLOR_GREEN;
				payload.state = LED_CMD_ON;
				payload.name = "Green LED Task";
				payload.priority = MEDIUM;
				//create_led_task(payload);
				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));
				taskENTER_CRITICAL();
				enqueue(&priorityQueue, item);
				taskEXIT_CRITICAL();
				xSemaphoreGive(xSemaphore);

				break;
			case BUTTON_STATE_LONG:
				LOGGER_INFO("Button long press detected");
				payload.color = LED_COLOR_BLUE;
				payload.state = LED_CMD_ON;
				payload.name = "Blue LED Task";
				payload.priority = LOW;
				//create_led_task(payload);
				item.priority = payload.priority;
				item.data = pvPortMalloc(sizeof(LedTask_t));
				memcpy(item.data, &payload, sizeof(LedTask_t));
				taskENTER_CRITICAL();
				enqueue(&priorityQueue, item);
				taskEXIT_CRITICAL();
				xSemaphoreGive(xSemaphore);

				break;
			default:
				break;
			}
		}

	}
}

void ui_send_message(message_t *pmsg){
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
