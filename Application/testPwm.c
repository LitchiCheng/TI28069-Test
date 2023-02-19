/*
 * testPwm.c
 *
 *  Created on: 2023年2月19日
 *      Author: 97478
 */

#include "testPwm.h"
#include "DSP28x_Project.h"     // Device Header file and Examples Include File
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void pwmTestTask(void * pvParameters){
    InitEPwm1Gpio();
    //CLKDIV = 1
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 2
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
    //增减计数模式
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    //计数器等于0，置高电平，计数值等于PRD，置低电平
    EPwm1Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm1Regs.TBPRD = 90000 /2;
    //%50占空比
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    while(1){
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


