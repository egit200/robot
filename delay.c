

#include "Arduino_FreeRTOS.h"
#include "delay.h"

inline void Task_Delay_Seconds(const uint16_t seconds) {
	vTaskDelay((seconds * 1000) / portTICK_PERIOD_MS);
}
