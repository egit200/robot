#include "Arduino_FreeRTOS.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "battery.h"
#include "panel.h"
#include "distance.h"
#include "serial.h"

#define SOLAR_PANEL_STACK_SIZE 256

/* Structure that will hold the TCB of the task being created. */
static StaticTask_t blink_task;

/* Buffer that the task being created will use as its stack. Note this is
   an array of StackType_t variables. The size of StackType_t is dependent on
   the RTOS port. */
static StackType_t blink_stack[SOLAR_PANEL_STACK_SIZE];

void Ctrl_Task_Blink(void *pvParameters) // This is a task.
{

  // initialize digital LED_BUILTIN on pin 13 as an output.
  //  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    PORTB ^= (1 << PIN5);
    // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    Task_Delay(1000 / portTICK_PERIOD_MS); // wait for one second
                                           // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    PORTB ^= (1 << PIN5);
    Task_Delay(3000 / portTICK_PERIOD_MS); // wait for one second
    Serial_String_New_Line("hola");
  }
}

// the setup function runs once when you press reset or power the board
int main(void)
{

  DDRB = 1 << PIN5;

  Serial_Init();
  Serial_String_New_Line("");
  Serial_String_New_Line("reset");

  // Battery_Monitor_Init();

  Solar_Panel_Init();

  // Distance_Sensor_Init();

  /*if (xTaskCreateStatic(
          Ctrl_Task_Blink,        /* Function that implements the task. *
          "Blink",                /* Text name for the task. *
          SOLAR_PANEL_STACK_SIZE, /* Number of indexes in the xStack array. *
          (void *)1,              /* Parameter passed into the task. *
          1,                      /* Priority at which the task is created. *
          blink_stack,            /* Array to use as the task's stack. *
          &blink_task) == NULL)
  {
    Serial_String_New_Line("error creadno tarea1");
  }*/

  Serial_String_New_Line("create");

  Serial_Put(123);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  Task_Start_Scheduler(); // Initialise and run the FreeRTOS scheduler. Execution should never return to here.
}

void vApplicationIdleHook(void) __attribute__((weak));

void vApplicationIdleHook(void)
{
}

void delay3(unsigned long ms)
{
}

void vApplicationMallocFailedHook(void) __attribute__((weak));

void vApplicationMallocFailedHook(void)
{
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char *pcTaskName) __attribute__((weak));

void vApplicationStackOverflowHook(TaskHandle_t xTask __attribute__((unused)),
                                   char *pcTaskName __attribute__((unused)))
{
  Serial_String_New_Line("overflow");
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *puxIdleTaskStackSize) __attribute__((weak));

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *puxIdleTaskStackSize)
{
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;
  *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
