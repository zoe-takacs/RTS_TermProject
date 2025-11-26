#include "crane.h"

void VertServo_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    VertServo_SetPulseUs(1500);
}

void RotServo_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    RotServo_SetPulseUs(1500);
}

static void Servo_WriteUs(TIM_HandleTypeDef *htim, uint32_t channel,
                          uint16_t us, uint16_t us_min, uint16_t us_max)
{
    if (us < us_min) us = us_min;
    if (us > us_max) us = us_max;
    __HAL_TIM_SET_COMPARE(htim, channel, us);
}

void VertServo_SetPulseUs(uint16_t us)
{
    Servo_WriteUs(&htim1, TIM_CHANNEL_1, us, VERT_PWM_MIN_US, VERT_PWM_MAX_US);
}

void RotServo_SetPulseUs(uint16_t us)
{
    Servo_WriteUs(&htim1, TIM_CHANNEL_2, us, ROT_PWM_MIN_US, ROT_PWM_MAX_US);
}
