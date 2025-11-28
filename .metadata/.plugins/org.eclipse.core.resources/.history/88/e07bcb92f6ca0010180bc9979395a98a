#include "crane.h"
#include "FreeRTOS.h"
#include "task.h"

void vert_motor_controller_task(void *argument)
{
    uint16_t pwm = 1500;
    VertServo_Init();

    for (;;)
    {
        if (!Read_Vert_Button())
        {
            VertServo_SetPulseUs(1500);
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        uint8_t dir = Read_Vert_Switch();
        float dist = HCSR04_ReadDistanceCm();

        if ((dist >= VERT_DIST_MAX_CM && dir == DIR_UP) ||
            (dist <= VERT_DIST_MIN_CM && dir == DIR_DOWN))
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        if (dir == DIR_UP)
            pwm += VERT_PWM_STEP_US;
        else
            pwm -= VERT_PWM_STEP_US;

        VertServo_SetPulseUs(pwm);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void rot_motor_controller_task(void *argument)
{
    uint16_t pwm = 1500;
    RotServo_Init();

    for (;;)
    {
        if (!Read_Rot_Button())
        {
            RotServo_SetPulseUs(1500);
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        uint8_t dir = Read_Rot_Switch();

        if (dir == DIR_RIGHT)
            pwm += ROT_PWM_STEP_US;
        else
            pwm -= ROT_PWM_STEP_US;

        RotServo_SetPulseUs(pwm);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
