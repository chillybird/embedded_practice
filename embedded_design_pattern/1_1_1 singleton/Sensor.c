#include "Sensor.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * 私有方法
 * */
void Sensor_Init(Sensor *const me) {}

void Sensor_Cleanup(Sensor *const me) {}

/**
 * 公共方法
 */
int Sensor_getFilterFrequency(const Sensor *const me)
{
    return me->filterFrequency;
}
void Sensor_setFilterFrequency(Sensor *const me, int p_filterFrequency)
{
    me->filterFrequency = p_filterFrequency;
}

int Sensor_getUpdateFrequency(const Sensor *me)
{
    return me->updateFrequency;
}

void Sensor_setUpdateFrequency(Sensor *const me, int p_updateFrequency)
{
    me->updateFrequency = p_updateFrequency;
}

int Sensor_getValue(const Sensor *const me)
{
    return me->value;
}

Sensor *Sensor_create(void)
{
    Sensor *me = (Sensor *)malloc(sizeof(Sensor));
    if (me != NULL)
    {
        Sensor_Init(me);
    }
}

void Sensor_Destroy(Sensor *const me)
{
    if (me != NULL)
    {
        Sensor_Cleanup(me);
    }
    free(me);
}
