#include "crane.h"

extern uint8_t capturing;
extern uint32_t echo_rising;
extern uint32_t echo_falling;

void HCSR04_Init(void)
{
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}

void delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

void trigger_pulse(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	delay_us(10); // 10 µs pulse
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
}

float HCSR04_ReadDistanceCm(void)
{
    capturing = 0;

    // Start with rising edge mode
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

    // Send pulse on trig pin
    trigger_pulse();

    uint32_t timeout = HAL_GetTick() + 50;
    while (capturing < 2)
    {
        if (HAL_GetTick() > timeout)
            return -1; // sensor failed
    }

    uint32_t pulse_width_us = echo_falling - echo_rising;

    return pulse_width_us * 0.0343f / 2.0f;
}
