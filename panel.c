#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "semphr.h"
#include "panel.h"
#include "serial.h"

#define SOLAR_PANEL_STACK_SIZE 192

static SemaphoreHandle_t solar_panel_semaphore_handle = NULL;
static StaticSemaphore_t solar_panel_mutex;
static BaseType_t solar_panel_voltage;

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t solar_panel_task;

/* Buffer that the task being created will use as its stack. Note this is
 an array of StackType_t variables. The size of StackType_t is dependent on
 the RTOS port. */
static StackType_t solar_panel_stack[SOLAR_PANEL_STACK_SIZE];

static const char CADENA[] PROGMEM = {"panel"};

void Solar_Panel_Init() {
	/* Create a mutex semaphore without using any dynamic memory allocation.
	 The mutex's data structures will be saved into the xMutexBuffer variable. */
	solar_panel_semaphore_handle = xSemaphoreCreateMutexStatic(
			&solar_panel_mutex);
	if (solar_panel_semaphore_handle == NULL) {
		Serial_String_New_Line("error panel mutex");
	}

	if (xTaskCreateStatic(Solar_Panel_Task, //  * Function that implements the task. *
			"panel",               // * Text name for the task. *
			SOLAR_PANEL_STACK_SIZE, //* Number of indexes in the xStack array. *
			(void*) 1,              //* Parameter passed into the task. *
			1,                      //* Priority at which the task is created. *
			solar_panel_stack,      //* Array to use as the task's stack. *
			&solar_panel_task) == NULL) {
		Serial_String_New_Line("error  panel");
	}
}

void Solar_Panel_Task(void *pvParameters) // This is a task.
{
	for (;;) // A Task shall never return or exit.
			{
		Serial_Put_String_Flash(CADENA);
		Serial_New_Line();

		xSemaphoreTake(solar_panel_semaphore_handle, portMAX_DELAY);
		solar_panel_voltage++;
		xSemaphoreGive(solar_panel_semaphore_handle);

		vTaskDelay(5000 / portTICK_PERIOD_MS); // wait for one second
	}
}

BaseType_t Solar_Panel_Get_Voltage() {
	BaseType_t return_value = 0;
	if (xSemaphoreTake(solar_panel_semaphore_handle, portMAX_DELAY) == pdTRUE) {
		return_value = solar_panel_voltage;
		xSemaphoreGive(solar_panel_semaphore_handle);
	}

	return return_value;
}
