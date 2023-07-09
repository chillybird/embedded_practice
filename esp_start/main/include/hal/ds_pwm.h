#ifndef __DS_PWM_H__
#define __DS_PWM_H__

typedef enum{
    BEEP_SHORT_100MS = 0,
    BEEP_SHORT_500MS,
    BEEP_LONG,
}BEEP_TYPE_E;

void ds_pwm_init(void);
void send_beep_event(BEEP_TYPE_E type);
void send_beep_event_from_isr(BEEP_TYPE_E type);

#endif // DS_PWM_H__