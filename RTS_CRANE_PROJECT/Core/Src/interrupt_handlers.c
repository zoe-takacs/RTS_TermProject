/*
 * interrupt_handlers.c
 *
 *  Created on: Nov 26, 2025
 *      Author: Zoe
 */

#include <stdint.h>
#include <main.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "crane.h"

extern TIM_HandleTypeDef htim2;
extern osThreadId_t vert_motor_controller_handle;
extern osThreadId_t rot_motor_controller_handle;
extern osThreadId_t auto_motor_controller_handle;

extern uint8_t capturing;
extern uint32_t echo_rising;
extern uint32_t echo_falling;

int mode = MANUAL;
char msg[50];
volatile uint8_t limit_tripped = 0;


void change_mode(void){
	mode = (mode + 1) % 3;
	switch(mode){
		case(MANUAL):
		snprintf(msg, sizeof(msg), "Mode = MANUAL\r\n");
			break;
		case(AUTO):
			snprintf(msg, sizeof(msg), "Mode = AUTO\r\n");
			break;
		case(CALIBRATION):
			snprintf(msg, sizeof(msg), "Mode = CALIBRATION\r\n");
			break;
		default:
			snprintf(msg, sizeof(msg), "Mode = UNKNOWN\r\n");
			break;
	}
	send_usart(msg);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC1);

        if (capturing == 0)
        {
            echo_rising = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

            TIM_IC_InitTypeDef sConfig = {0};
            sConfig.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
            sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
            HAL_TIM_IC_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);

            capturing = 1;
        }
        else if (capturing == 1)
        {
            echo_falling = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
            capturing = 2;

            // restore rising edge for next measurement
            TIM_IC_InitTypeDef sConfig = {0};
            sConfig.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
            sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
            HAL_TIM_IC_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // ===== Limit switch on PB14 (LIMIT_SW_Pin) =====
    if (GPIO_Pin == LIMIT_SW_Pin)
    {
        // Latch the fault
        limit_tripped = 1;

        // Immediately force both servos to neutral / stop position
        VertServo_SetPulseUs(1500);
        RotServo_SetPulseUs(1500);

        // Once the limit is hit, ignore all other button presses
        return;
    }

    // If the limit has already been tripped, ignore other EXTI sources
    if (limit_tripped)
    {
        return;
    }

    // ===== Mode / button handling (existing behaviour) =====
    if (GPIO_Pin == mode_btn_Pin)
    {
        change_mode();
    }
    else if (GPIO_Pin == vert_btn_Pin && mode == MANUAL)
    {
        // Resume task from ISR
        xHigherPriorityTaskWoken = xTaskResumeFromISR(vert_motor_controller_handle);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else if (GPIO_Pin == rot_btn_Pin && mode == MANUAL)
    {
        xHigherPriorityTaskWoken = xTaskResumeFromISR(rot_motor_controller_handle);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else if (GPIO_Pin == auto_btn_Pin && mode == AUTO)
    {
        xHigherPriorityTaskWoken = xTaskResumeFromISR(auto_motor_controller_handle);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
