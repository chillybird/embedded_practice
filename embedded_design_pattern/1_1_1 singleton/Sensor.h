#ifndef __SENSOR_H__
#define __SENSOR_H__

typedef struct Sensor Sensor;
struct Sensor {
    int filterFrequency;
    int updateFrequency;
    int value;
};

int Sensor_getFilterFrequency(const Sensor* const me);
void Sensor_setFilterFrequency(Sensor* const me, int p_filterFrequency);
int Sensor_getUpdateFrequency(const Sensor* me);
void Sensor_setUpdateFrequency(Sensor* const me, int p_updateFrequency);
int Sensor_getValue(const Sensor* const me);
Sensor* Sensor_create(void);
void Sensor_Destroy(Sensor* const me);

#endif // SENSOR_H__