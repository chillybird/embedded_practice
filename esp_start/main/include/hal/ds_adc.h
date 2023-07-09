#ifndef __DS_ADC_H__
#define __DS_ADC_H__

#include <stdint.h>

uint32_t getBatteryLevel(void);
void ds_batteryLevelMonitor_init(void);

#endif // DS_ADC_H__