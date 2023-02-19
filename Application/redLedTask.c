/*
 * redLedTask.c
 *
 *  Created on: 2023Äê2ÔÂ19ÈÕ
 *      Author: 97478
 */

//-------------------------------------------------------------------------------------------------

#include "redLedTask.h"
#include "DSP28x_Project.h"     // Device Header file and Examples Include File
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static SemaphoreHandle_t xSemaphore = NULL;
static StaticSemaphore_t xSemaphoreBuffer;

interrupt void timer1_ISR( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

//-------------------------------------------------------------------------------------------------
static void setupTimer1( void )
{
    // Start the timer than activate timer interrupt to switch into first task.
    EALLOW;
    PieVectTable.TINT1 = &timer1_ISR;
    EDIS;

    ConfigCpuTimer(&CpuTimer1,
                   configCPU_CLOCK_HZ / 1000000,  // CPU clock in MHz
                   100000);                       // Timer period in uS
    CpuTimer1Regs.TCR.all = 0x4000;               // Enable interrupt and start timer

    IER |= M_INT13;
}

static void redLedToggle(void)
{
    static uint32_t counter = 0;

    counter++;
    if(counter & 1)
    {
        GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
    }
    else
    {
        GpioDataRegs.GPBSET.bit.GPIO34 = 1;
    }
}

void LED_TaskRed(void * pvParameters)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
    EDIS;

    setupTimer1();
    xSemaphore = xSemaphoreCreateBinaryStatic( &xSemaphoreBuffer );

    for(;;)
    {
        if(xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE)
        {
            redLedToggle();
        }
    }
}

