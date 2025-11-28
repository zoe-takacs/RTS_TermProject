/*
 * comm_io.c
 *
 *  Created on: Nov 26, 2025
 *      Author: Zoe
 */

#include "usart.h"
#include <string.h>
#include "crane.h"
#include "FreeRTOS.h"
#include "queue.h"

#define USART_MSG_LEN 50
extern QueueHandle_t usartQueue;

void send_usart(char *msg)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void usart_task(void *arg)
{
    char msg[USART_MSG_LEN];
    for (;;)
    {
        if (xQueueReceive(usartQueue, &msg, portMAX_DELAY) == pdTRUE)
        {
            send_usart(msg); // actual transmission in task context
        }
    }
}
