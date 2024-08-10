/**
 * @file active_object_led.h
 * @brief This file declares the LED active object.
 * @author RTOSII - Grupo 5
 */

/* ============================================================================================ */

#ifndef INC_ACTIVE_OBJECT_LED_H
#define INC_ACTIVE_OBJECT_LED_H

/* ============================================================================================ */

#include "main.h"
#include "cmsis_os.h"
#include "task_led.h"
#include "priority_queue.h"

/* ============================================================================================ */

#define LED_ON_TIME 5000 // Time in milliseconds to keep the LED on
#define LED_TASK_STACK_SIZE configMINIMAL_STACK_SIZE // Stack size for the LED task

/* ============================================================================================ */

/* This enum defines the possible commands for the LED */
typedef enum
{
    LED_CMD_OFF, 
    LED_CMD_ON
} led_cmd_t;

/* This struct defines the LED task */
typedef struct
{
	led_color_t color; // Color of the LED
	led_cmd_t state ;  // State of the LED
	char* name; 	   // Name of the task
	Priority priority; // Priority of the task
} LedTask_t;

/* LED events queues */
extern QueueHandle_t led_event_queue;

/* LED tasks */
extern LedTask_t led_task;

/* ============================================================================================ */

/**
 * @brief This function runs the LED task
 */
void led_task_run(void);

/* ============================================================================================ */

/**
 * @brief This function creates a LED task
 * @param event_queue This is the queue to receive commands
 * @param set_state This is a function pointer to set the LED state
 * @param task_name This is the name of the task
 */
void create_led_task(const char *task_name);

/* ============================================================================================ */

/**
 * @brief This function sets the red LED state
 * @param cmd This is the command to set the LED state
 */
void led_red_set_state(led_cmd_t cmd);

/* ============================================================================================ */

/**
 * @brief This function sets the green LED state
 * @param cmd This is the command to set the LED state
 */
void led_green_set_state(led_cmd_t cmd);

/* ============================================================================================ */

/**
 * @brief This function sets the blue LED state
 * @param cmd This is the command to set the LED state
 */
void led_blue_set_state(led_cmd_t cmd);

/* ============================================================================================ */

#endif // ACTIVE_OBJECT_LED_H