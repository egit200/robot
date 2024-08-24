#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "semphr.h"
#include "battery.h"
#include "serial.h"

#define SOLAR_PANEL_STACK_SIZE 192

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t battery_monitor_task;

/* Buffer that the task being created will use as its stack. Note this is
   an array of StackType_t variables. The size of StackType_t is dependent on
   the RTOS port. */
static StackType_t battery_monitor_stack[SOLAR_PANEL_STACK_SIZE];

static SemaphoreHandle_t semaphore_handle = NULL;
static StaticSemaphore_t mutex;
static BaseType_t voltage;

void Battery_Monitor_Init()
{
    /* Create a mutex semaphore without using any dynamic memory allocation.
    The mutex's data structures will be saved into the xMutexBuffer variable. */
    semaphore_handle = xSemaphoreCreateMutexStatic(&mutex);

    xTaskCreateStatic(
        Battery_Monitor_Task,   /* Function that implements the task. */
        "bat",                  /* Text name for the task. */
        SOLAR_PANEL_STACK_SIZE, /* Number of indexes in the xStack array. */
        (void *)1,              /* Parameter passed into the task. */
        tskIDLE_PRIORITY+1,       /* Priority at which the task is created. */
        battery_monitor_stack,  /* Array to use as the task's stack. */
        &battery_monitor_task);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Battery_Monitor_Task(void *pvParameters) // This is a task.
{
    for (;;) // A Task shall never return or exit.
    {
        if (semaphore_handle != NULL)
        {
            /* See if we can obtain the semaphore. If the semaphore is not
               available wait 10 ticks to see if it becomes free. */
            if (xSemaphoreTake(semaphore_handle, portMAX_DELAY) == pdTRUE)
            {
                /* We were able to obtain the semaphore and can now access the
                   shared resource. */

                /* ... */

                /* We have finished accessing the shared resource. Release the
                   semaphore. */
                xSemaphoreGive(semaphore_handle);
            }
            else
            {
                /* We could not obtain the semaphore and can therefore not access
                   the shared resource safely. */
            }
        }
        Serial_String_New_Line("bat");
        vTaskDelay(5000 / portTICK_PERIOD_MS); // wait for one second
    }
}

BaseType_t Battery_Monitor_Get_Voltage()
{
    BaseType_t return_value = 0;
    if (xSemaphoreTake(semaphore_handle, portMAX_DELAY) == pdTRUE)
    {
        return_value = voltage;
        xSemaphoreGive(semaphore_handle);
    }

    return return_value;
}

BaseType_t Battery_Monitor_Is_Charging()
{
    BaseType_t return_value = 0;
    if (xSemaphoreTake(semaphore_handle, portMAX_DELAY) == pdTRUE)
    {
        return_value = voltage;
        xSemaphoreGive(semaphore_handle);
    }

    return return_value;
}
