#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "semphr.h"

#include "distance.h"
#include "serial.h"

#define SOLAR_PANEL_STACK_SIZE 192

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t distance_sensor_task;

/* Buffer that the task being created will use as its stack. Note this is
   an array of StackType_t variables. The size of StackType_t is dependent on
   the RTOS port. */
static StackType_t distance_sensor_stack[SOLAR_PANEL_STACK_SIZE];
static SemaphoreHandle_t semaphore_handle = NULL;
static StaticSemaphore_t mutex;
static BaseType_t distance;

void Distance_Sensor_Init()
{
    /* Create a mutex semaphore without using any dynamic memory allocation.
    The mutex's data structures will be saved into the xMutexBuffer variable. */
    semaphore_handle = xSemaphoreCreateMutexStatic(&mutex);

    xTaskCreateStatic(
        Distance_Sensor_Task,   /* Function that implements the task. */
        "panel",                /* Text name for the task. */
        SOLAR_PANEL_STACK_SIZE, /* Number of indexes in the xStack array. */
        (void *)1,              /* Parameter passed into the task. */
        tskIDLE_PRIORITY,       /* Priority at which the task is created. */
        distance_sensor_stack,  /* Array to use as the task's stack. */
        &distance_sensor_task);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Distance_Sensor_Task(void *pvParameters) // This is a task.
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

        Serial_String_New_Line("dist");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
    }
}

BaseType_t Distance_Sensor_Get_Distance()
{
    BaseType_t return_value = 0;
    if (xSemaphoreTake(semaphore_handle, portMAX_DELAY) == pdTRUE)
    {
        return_value = distance;
        xSemaphoreGive(semaphore_handle);
    }

    return return_value;
}
