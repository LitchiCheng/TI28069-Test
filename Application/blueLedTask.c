/*
 * blueLedTask.c
 *
 *  Created on: 2023Äê2ÔÂ19ÈÕ
 *      Author: 97478
 */

#include "blueLedTask.h"
#include "DSP28x_Project.h"     // Device Header file and Examples Include File
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static void blueLedToggle(void)
{
    static uint32_t counter = 0;

    counter++;
    if(counter & 1)
    {
        GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
    }
    else
    {
        GpioDataRegs.GPBSET.bit.GPIO39 = 1;
    }
}

void LED_TaskBlue(void * pvParameters)
{
    for(;;)
    {
        blueLedToggle();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}





