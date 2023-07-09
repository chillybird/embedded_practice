#ifndef __MOTORDATA_H__
#define __MOTORDATA_H__

#include "HWProxyExample.h"

typedef struct MotorData MotorData;
struct MotorData
{
    unsigned char on_off;
    DirectionType direction;
    unsigned int speed;
    unsigned char errorStatus;
    unsigned char noPowerError;
    unsigned char noTorqueError;
    unsigned char BITError;
    unsigned char overTemperatureError;
    unsigned char reservedError1;
    unsigned char reservedError2;
    unsigned char unknownError;
};

#endif // MOTORDATA_H__