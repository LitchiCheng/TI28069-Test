/*
 * testPwm.c
 *
 *  Created on: 2023��2��19��
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
    //��������ģʽ
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    //����������0���øߵ�ƽ������ֵ����PRD���õ͵�ƽ
    EPwm1Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm1Regs.TBPRD = 90000 /2;
    //%50ռ�ձ�
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    while(1){
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


