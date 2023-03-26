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
#include "testConfig.h"

//测试中有3个pwm独立输出、3个pwm独立且有相位差的输出
void pwmTestTask(void * pvParameters){
    /***************************/
    #ifdef PWM1_TEST
    InitEPwm1Gpio();
    //CLKDIV = 1
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 2
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
    //增减计数模式
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;

    #ifdef PWM1_NORMAL_TEST
    //计数器等于0，置高电平，计数值等于PRD，置低电平
    EPwm1Regs.AQCTLA.all = 0x0006;
    #endif

    #ifdef PWM1_PULSE_MUTABLE_TEST
    //增计数时，计数器等于CMPA，PWM1A置高电平；减计数时，计数值等于CMPA，PWM1A置低电平
    EPwm1Regs.AQCTLA.all = 0x0060;
    #endif

    #ifdef PWM1_A_B_COMPLEMENTARY
    //增计数时，计数器等于CMPA，PWM1A置低电平；减计数时，计数值等于CMPA，PWM1B置高电平
    EPwm1Regs.AQCTLB.all = 0x0090;
    #endif
    //1khz PWM
    EPwm1Regs.TBPRD = 90000 /2;

    #ifdef PWM1_NORMAL_TEST
    //%50占空比
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    #endif

    #ifdef PWM1_PULSE_MUTABLE_TEST
    //占空比100%
    EPwm1Regs.CMPA.half.CMPA = 0;
    #endif

    #ifdef PWM_DEAD_BAND_TEST
    //增计数时，计数器等于CMPA，PWM1A置高电平；减计数时，计数值等于CMPA，PWM1A置低电平
    EPwm1Regs.AQCTLA.all = 0x0060;
    //%50占空比
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    #endif

    #ifdef PWM_CHOPPER_TEST
    //增计数时，计数器等于CMPA，PWM1B置高电平；减计数时，计数值等于CMPA，PWM1B置低电平
    EPwm1Regs.AQCTLB.all = 0x0600;
    //50%占空比
    EPwm1Regs.CMPB = 90000/4;
    #endif

    #endif
    /***************************/
    #ifdef PWM2_TEST
    InitEPwm2Gpio();
    //CLKDIV = 1
    EPwm2Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 2
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;
    //增减计数模式
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;
    //计数器等于0，置高电平，计数值等于PRD，置低电平
    EPwm2Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm2Regs.TBPRD = 90000 /2;
    //%50占空比
    EPwm2Regs.CMPA.half.CMPA = 90000/4;
    #endif
    /***************************/
    #ifdef PWM3_TEST
    InitEPwm3Gpio();
    //CLKDIV = 1
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 1
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
    //增减计数模式
    EPwm3Regs.TBCTL.bit.CTRMODE = 2;
    //计数器等于0，置高电平，计数值等于PRD，置低电平
    EPwm3Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm3Regs.TBPRD = 90000 /2;
    //%50占空比
    EPwm3Regs.CMPA.half.CMPA = 90000/4;
    #endif
    /***************************/

    #ifdef PWM_PHASE_TEST
    //当CTR为0时，同步输出
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;
    //使能EPWM2相移
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    //产生同步相位
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;
    //相位1/3，即120°
    EPwm2Regs.TBPHS.half.TBPHS = 90000 / 2 /3;
    //使能EPWM3相移
    EPwm3Regs.TBCTL.bit.PHSEN = 1;
    //相位240°
    EPwm3Regs.TBPHS.half.TBPHS = 90000 / 3;
    #endif

    #ifdef PWM_DEAD_BAND_TEST
    EPwm1Regs.DBRED = 750;              // 上升沿延时10 ms
    EPwm1Regs.DBFED = 750;              // 下降沿延时10 ms
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;   // ePWM1A = RED
    EPwm1Regs.DBCTL.bit.POLSEL = 2;     // S3=1 ePWM1B反相信号
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;    // ePWM1A 作为输入信号
    #endif

    #ifdef PWM_CHOPPER_TEST
    EPwm1Regs.PCCTL.bit.CHPFREQ = 7;   // SYSCLKOUT / 64  = 2.34375MHz
    EPwm1Regs.PCCTL.bit.CHPDUTY = 3;    // 50% 占空比
    EPwm1Regs.PCCTL.bit.CHPEN = 1;      // 使能斩波模式
    EPwm1Regs.PCCTL.bit.OSHTWTH = 14;   // 120 *6.67 ns = 800 ns
    #endif

    int updown = 1;
    while(1){
        #ifdef  PWM1_PULSE_MUTABLE_TEST
        if(updown){
            if(EPwm1Regs.CMPA.half.CMPA < EPwm1Regs.TBPRD){
                EPwm1Regs.CMPA.half.CMPA+=90000/16;
            }else{
                updown = 0;
            }
        }else{
            if(EPwm1Regs.CMPA.half.CMPA > 0){
                EPwm1Regs.CMPA.half.CMPA-=90000/16;
            }else{
                updown = 1;
            }
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
        #endif
    }
}


