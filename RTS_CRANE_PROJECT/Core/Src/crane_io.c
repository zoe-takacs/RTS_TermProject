#include "crane.h"
#include "main.h"

uint8_t Read_Vert_Button(void)
{
    return HAL_GPIO_ReadPin(vert_btn_GPIO_Port, vert_btn_Pin) == GPIO_PIN_RESET;
}

uint8_t Read_Rot_Button(void)
{
    return HAL_GPIO_ReadPin(rot_btn_GPIO_Port, rot_btn_Pin) == GPIO_PIN_RESET;
}

uint8_t Read_Vert_Switch(void)
{
    return (HAL_GPIO_ReadPin(vert_sw_GPIO_Port, vert_sw_Pin) == GPIO_PIN_SET) ? DIR_UP : DIR_DOWN;
}

uint8_t Read_Rot_Switch(void)
{
    return (HAL_GPIO_ReadPin(rot_sw_GPIO_Port, rot_sw_Pin) == GPIO_PIN_SET) ? DIR_RIGHT : DIR_LEFT;
}
