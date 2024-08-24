//#include "list.h"

#include "task-priority.h"



Task_Priority Task_Get_Idle_Priority()
{
    return 0;
}

Task_Priority Task_Get_Max_Priority()
{
    //return (Task_Priority) configMAX_PRIORITIES;
     return (Task_Priority) 4;
}
