#ifndef _BATTERY_MONITOR_H_
#define _BATTERY_MONITOR_H_

#include "Arduino_FreeRTOS.h"

extern void Battery_Monitor_Init();

extern void Battery_Monitor_Task(void *parameters);

extern BaseType_t Battery_Monitor_Get_Voltage();

extern BaseType_t Battery_Monitor_Is_Charging();

#endif
