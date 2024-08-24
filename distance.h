#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include "Arduino_FreeRTOS.h"

extern void Distance_Sensor_Init();

extern void Distance_Sensor_Task(void *parameters);

extern BaseType_t Distance_Sensor_Get_Distance();

#endif
