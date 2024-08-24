
#ifndef TASK_PRIORITY_H
#define TASK_PRIORITY_H

#include <avr/wdt.h>

//#include "portmacro.h"

typedef uint8_t Task_Priority;

 Task_Priority Task_Get_Idle_Priority() ;

 Task_Priority Task_Get_Max_Priority() ;

#endif //TASK_PRIORITY_H