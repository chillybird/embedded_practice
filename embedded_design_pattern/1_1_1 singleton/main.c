#include "Sensor.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    Sensor* p_Sensor0, *p_Sensor1;
    p_Sensor0 = Sensor_create();
    p_Sensor1 = Sensor_create();

    p_Sensor0->value = 99;
    p_Sensor1->value = -1;
    printf("value from Sensor 0 : %d\n", Sensor_getValue(p_Sensor0));
    printf("value from Sensor 1 : %d\n", Sensor_getValue(p_Sensor1));

    Sensor_Destroy(p_Sensor0);
    Sensor_Destroy(p_Sensor1);

    return 0;
}