//
// Created by zhang on 2023/8/18.
//

#include "func_test.h"

uint8_t I2C_Buff_Write[DATA_Size];
uint8_t I2C_Buff_Read[DATA_Size];

int waveform = 0;        // current waveform
int waveforms_max = 0;   // max waveforms available (we'll ask...)

void test_led()
{
    static uint8_t t;
    for (t = 0; t <= 100; t++) {
        LED_Control(t, 100 - t, 0); // 绿-红
        HAL_Delay(20);
    }

    for (t = 0; t <= 100; t++) {
        LED_Control(100 - t, 0, t); // 红-蓝
        HAL_Delay(20);
    }

    for (t = 0; t <= 100; t++) {
        LED_Control(0, t, 100 - t); // 蓝-绿
        HAL_Delay(20);
    }
}

void test_uart()
{
    static UART_MESS mess;
    static uint32_t tick_now, tick_latest;
    static uint8_t mess_data[128];
    if(rt_ringbuffer_data_len(&mess_buffer))
    {
        rt_ringbuffer_get(&mess_buffer, (uint8_t*)&mess.msg_type, 2);
        switch (mess.msg_type) {
            case MSG_STR:
                rt_ringbuffer_get(&usart_buffer, mess_data, mess.msg_len);
                mess_data[mess.msg_len] = '\0';
                printf("%s\n", mess_data);
                break;
            default:
                break;
        }
    }

    tick_now = HAL_GetTick() / (5000U / uwTickFreq); // 1s
    if (tick_now != tick_latest)
    {
        printf("time %lu s.\r\n", 5 * tick_now);
        tick_latest = tick_now;
    }
}

//void test_iwdg()
//{
//    static uint32_t tick_now, tick_latest;
//    tick_now = HAL_GetTick() / (1000U / uwTickFreq); // 1s
//    if (tick_now != tick_latest)
//    {
//        HAL_IWDG_Refresh(&hiwdg);
////        printf("Refresh IWDG.\n");
//        tick_latest = tick_now;
//    }
//}

uint8_t test_eeprom()
{
    uint16_t i;

    EEPROM_DEBUG("Write Data\n");
    for (i = 0; i < DATA_Size; i++)
    {
        I2C_Buff_Write[i] = i;
        printf("0x%02X ", I2C_Buff_Write[i]);
        if(i % 16 == 15)
            printf("\n\r");
    }
    I2C_EE_BufferWrite(I2C_Buff_Write, EEP_Firstpage, DATA_Size);

    EEPROM_DEBUG("Read Data\n");
    I2C_EE_BufferRead(I2C_Buff_Read, EEP_Firstpage, DATA_Size);

    for (i = 0; i < DATA_Size; i++)
    {
        printf("0x%02X ", I2C_Buff_Read[i]);
        if(I2C_Buff_Read[i] != I2C_Buff_Write[i])
        {
            EEPROM_DEBUG("Data storage error\n");
            return 0;
        }
        if(i % 16 == 15)
            printf("\n\r");
    }
    EEPROM_DEBUG("EEPROM test completed\n");
    return 1;
}

void test_drv2605(uint8_t waveform)
{
    printf("Waveform # %d", waveform);
    Haptic_DRV2605_setWaveform(0, waveform);
    Haptic_DRV2605_setWaveform(1, 0);
    Haptic_DRV2605_goWait();
    HAL_Delay(500);
    waveform++;
    if (waveform >= waveforms_max) waveform = 0;
}

void drv2605_Init()
{
    printf("Device ID 0x%02x\n", Haptic_DRV2605_getDeviceID());

    if (Haptic_DRV2605_begin() != HAPTIC_SUCCESS)
    {
        printf("Initialize DRV2605 Fail.\n");
    }
    else
    {
        Haptic_DRV2605_setActuatorType(LRA);
        Haptic_DRV2605_setMode(REGISTER_MODE);
        waveforms_max = Haptic_DRV2605_getWaveforms();

        printf("Max waveform %d\n", waveforms_max);
    }
}

void test_mpu6050()
{
    short Acel[3];
    short Gyro[3];
    float Temp;
    MPU6050_Init();
    if(MPU6050ReadID() == 1)
    {
        while(1)
        {
            MPU6050ReadAcc(Acel);
            printf("Acc : %8d%8d%8d ",Acel[0],Acel[1],Acel[2]);
            MPU6050ReadGyro(Gyro);
            printf("     Gyro : %8d%8d%8d ",Gyro[0],Gyro[1],Gyro[2]);
            MPU6050_ReturnTemp(&Temp);
            printf("     Temp : %8.2f\r\n",Temp);
            HAL_Delay(100);
        }
    }
    else
    {
        while(1);
    }
}
