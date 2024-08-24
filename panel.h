
#ifndef _SOLAR_PANEL_H_
#define _SOLAR_PANEL_H_


extern void Solar_Panel_Init();
extern void Solar_Panel_Task(void *pvParameters);
extern BaseType_t Solar_Panel_Get_Voltage();

#endif
