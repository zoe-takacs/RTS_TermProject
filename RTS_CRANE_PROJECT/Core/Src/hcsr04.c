#include "crane.h"

void HCSR04_Init(void)
{
    HAL_TIM_Base_Start(&htim2);
}

float HCSR04_ReadDistanceCm(void)
{
    uint32_t timeout_us = 30000;

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);
    HAL_Delay(2);

    uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);

    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_SET);
    while ((__HAL_TIM_GET_COUNTER(&htim2) - start) < 10);
    HAL_GPIO_WritePin(HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin, GPIO_PIN_RESET);

    uint32_t wait = __HAL_TIM_GET_COUNTER(&htim2);
    while (!HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin))
    {
        if ((__HAL_TIM_GET_COUNTER(&htim2) - wait) > timeout_us)
            return -1;
    }

    uint32_t pulse_start = __HAL_TIM_GET_COUNTER(&htim2);

    while (HAL_GPIO_ReadPin(HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin))
    {
        if ((__HAL_TIM_GET_COUNTER(&htim2) - pulse_start) > timeout_us)
            return -1;
    }

    uint32_t pulse_end = __HAL_TIM_GET_COUNTER(&htim2);

    uint32_t width = (pulse_end >= pulse_start)
                     ? (pulse_end - pulse_start)
                     : (0xFFFFFFFFUL - pulse_start + pulse_end);

    return width / 58.0f;
}
