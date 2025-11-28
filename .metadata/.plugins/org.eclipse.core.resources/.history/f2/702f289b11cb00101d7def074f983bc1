#include "crane.h"

uint8_t Read_Vert_Button(void)
{
    return HAL_GPIO_ReadPin(VERT_BTN_GPIO_Port, VERT_BTN_Pin) == GPIO_PIN_RESET;
}

uint8_t Read_Rot_Button(void)
{
    return HAL_GPIO_ReadPin(ROT_BTN_GPIO_Port, ROT_BTN_Pin) == GPIO_PIN_RESET;
}

uint8_t Read_Vert_Switch(void)
{
    return (HAL_GPIO_ReadPin(VERT_SW_GPIO_Port, VERT_SW_Pin) == GPIO_PIN_SET) ? DIR_UP : DIR_DOWN;
}

uint8_t Read_Rot_Switch(void)
{
    return (HAL_GPIO_ReadPin(ROT_SW_GPIO_Port, ROT_SW_Pin) == GPIO_PIN_SET) ? DIR_RIGHT : DIR_LEFT;
}
