#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "semphr.h"
#include "panel.h"
#include "serial.h"

#define SOLAR_PANEL_STACK_SIZE 200

/* Structure that will hold the TCB of the task being created. */
StaticTask_t solar_panel_task;

/* Buffer that the task being created will use as its stack. Note this is
   an array of StackType_t variables. The size of StackType_t is dependent on
   the RTOS port. */
StackType_t solar_panel_stack[SOLAR_PANEL_STACK_SIZE];

SemaphoreHandle_t solar_panel_semaphore_handle = NULL;
StaticSemaphore_t solar_panel_mutex;
BaseType_t solar_panel_voltage;

void Solar_Panel_Init()
{
    Serial_String_New_Line("Solar_Panel_Init");
    /* Create a mutex semaphore without using any dynamic memory allocation.
    The mutex's data structures will be saved into the xMutexBuffer variable. */
    /* solar_panel_semaphore_handle = xSemaphoreCreateMutexStatic(&solar_panel_mutex);
    if (solar_panel_semaphore_handle == NULL)
    {
        Serial_String_New_Line("error panel mutex");
    }
    */

    if (xTaskCreateStatic(
            Solar_Panel_Init,       /* Function that implements the task. */
            "panel",                /* Text name for the task. */
            SOLAR_PANEL_STACK_SIZE, /* Number of indexes in the xStack array. */
            (void *)1,              /* Parameter passed into the task. */
            1,                      /* Priority at which the task is created. */
            solar_panel_stack,      /* Array to use as the task's stack. */
            &solar_panel_task) == NULL)
    {
        Serial_String_New_Line("error  panel");
    }
}

void Solar_Panel_Task(void *pvParameters) // This is a task.
{
    Serial_String_New_Line("inicio panel");

    for (;;) // A Task shall never return or exit.
    {
        /* if (solar_panel_semaphore_handle != NULL)
        {
            * See if we can obtain the semaphore. If the semaphore is not
               available wait 10 ticks to see if it becomes free. *
            if (xSemaphoreTake(solar_panel_semaphore_handle, portMAX_DELAY) == pdTRUE)
            {
                * We were able to obtain the semaphore and can now access the
                   shared resource. *

        * ... *

        * We have finished accessing the shared resource. Release the
           semaphore. *
        xSemaphoreGive(solar_panel_semaphore_handle);
    }
    else
    {


        * We could not obtain the semaphore and can therefore not access
           the shared resource safely. *
    }
}*/
        PORTB ^= (1 << PIN5);
        // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        Task_Delay(1000 / portTICK_PERIOD_MS); // wait for one second
                                               // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        PORTB ^= (1 << PIN5);
        Task_Delay(3000 / portTICK_PERIOD_MS); // wait for one second
        Serial_String_New_Line("panel");
    }
}

BaseType_t Solar_Panel_Get_Voltage()
{
    BaseType_t return_value = 0;
    if (xSemaphoreTake(solar_panel_semaphore_handle, portMAX_DELAY) == pdTRUE)
    {
        return_value = solar_panel_voltage;
        xSemaphoreGive(solar_panel_semaphore_handle);
    }

    return return_value;
}