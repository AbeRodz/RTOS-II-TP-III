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
 * @authors : Abraham Rodriguez, Estanislao Crivos, Jose Roberto Castro
 */

/********************** inclusions *******************************************/

#include "app.h"
#include "priority_queue.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

LedTask_t led_task;
UiTask_t ui_task;
SemaphoreHandle_t xSemaphore;

/********************** external data declaration *****************************/

/********************** external functions definition ************************/

void app_init(void)
{
    /* Create button task */
    ui_task.led_task = &led_task;

    /* Create UI task */
    ui_task_create(&ui_task);

    /* Create LED task */
    BaseType_t status;

    /* Create the LED tasks */
    create_led_task("LED_AO");

    /* Initialize the priority queue */
    initQueue(&priorityQueue);

    /* Create the counting semaphore */
    xSemaphore = xSemaphoreCreateCounting(MAX_QUEUE_SIZE, 0);
    if (xSemaphore == NULL) {

        while (1);
    }
    
    /* Create button task */
    status = xTaskCreate(task_button,
    					"Button Task",
						configMINIMAL_STACK_SIZE,
						ui_task.button_state_queue,
						tskIDLE_PRIORITY,
						NULL);

    configASSERT(status == pdPASS);
	LOGGER_INFO("App initialized...");
	cycle_counter_init();
}

/********************** end of file ******************************************/
