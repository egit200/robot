#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "semphr.h"
#include "ctrl.h"
#include "panel.h"
#include "serial.h"
#include "delay.h"

#define SOLAR_PANEL_STACK_SIZE 192

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t controller_task;

/* Buffer that the task being created will use as its stack. Note this is
 an array of StackType_t variables. The size of StackType_t is dependent on
 the RTOS port. */
static StackType_t controller_stack[SOLAR_PANEL_STACK_SIZE];

static const char CADENA[] PROGMEM = {"controller"};

void Controller_Init() {
	/* Create a mutex semaphore without using any dynamic memory allocation.
	 The mutex's data structures will be saved into the xMutexBuffer variable. */

	if (xTaskCreateStatic(Controller_Task, //  * Function that implements the task. *
			"controller",               // * Text name for the task. *
			SOLAR_PANEL_STACK_SIZE, //* Number of indexes in the xStack array. *
			(void*) 1,              //* Parameter passed into the task. *
			3,                      //* Priority at which the task is created. *
			controller_stack,      //* Array to use as the task's stack. *
			&controller_task) == NULL) {
		Serial_String_New_Line("error  panel");
	}
}

void Controller_Task(void *parameters) // This is a task.
{
	for (;;) {
		TickType_t ticks = xTaskGetTickCount();
		Serial_Put_Long(ticks);
		Serial_String(" ");
		Serial_Put_String_Flash(CADENA);
		Serial_String(" ");
		BaseType_t voltage = Solar_Panel_Get_Voltage();
		Serial_Put(voltage);
		Serial_New_Line();

		Task_Delay_Seconds(1); // wait for one second
	}
}

