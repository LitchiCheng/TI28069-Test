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
#include "testConfig.h"

//��������3��pwm���������3��pwm����������λ������
void pwmTestTask(void * pvParameters){
    /***************************/
    #ifdef PWM1_TEST
    InitEPwm1Gpio();
    //CLKDIV = 1
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 2
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
    //��������ģʽ
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;

    #ifdef PWM1_NORMAL_TEST
    //����������0���øߵ�ƽ������ֵ����PRD���õ͵�ƽ
    EPwm1Regs.AQCTLA.all = 0x0006;
    #endif

    #ifdef PWM1_PULSE_MUTABLE_TEST
    //������ʱ������������CMPA��PWM1A�øߵ�ƽ��������ʱ������ֵ����CMPA��PWM1A�õ͵�ƽ
    EPwm1Regs.AQCTLA.all = 0x0060;
    #endif

    #ifdef PWM1_A_B_COMPLEMENTARY
    //������ʱ������������CMPA��PWM1A�õ͵�ƽ��������ʱ������ֵ����CMPA��PWM1B�øߵ�ƽ
    EPwm1Regs.AQCTLB.all = 0x0090;
    #endif
    //1khz PWM
    EPwm1Regs.TBPRD = 90000 /2;

    #ifdef PWM1_NORMAL_TEST
    //%50ռ�ձ�
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    #endif

    #ifdef PWM1_PULSE_MUTABLE_TEST
    //ռ�ձ�100%
    EPwm1Regs.CMPA.half.CMPA = 0;
    #endif

    #ifdef PWM_DEAD_BAND_TEST
    //������ʱ������������CMPA��PWM1A�øߵ�ƽ��������ʱ������ֵ����CMPA��PWM1A�õ͵�ƽ
    EPwm1Regs.AQCTLA.all = 0x0060;
    //%50ռ�ձ�
    EPwm1Regs.CMPA.half.CMPA = 90000/4;
    #endif

    #ifdef PWM_CHOPPER_TEST
    //������ʱ������������CMPA��PWM1B�øߵ�ƽ��������ʱ������ֵ����CMPA��PWM1B�õ͵�ƽ
    EPwm1Regs.AQCTLB.all = 0x0600;
    //50%ռ�ձ�
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
    //��������ģʽ
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;
    //����������0���øߵ�ƽ������ֵ����PRD���õ͵�ƽ
    EPwm2Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm2Regs.TBPRD = 90000 /2;
    //%50ռ�ձ�
    EPwm2Regs.CMPA.half.CMPA = 90000/4;
    #endif
    /***************************/
    #ifdef PWM3_TEST
    InitEPwm3Gpio();
    //CLKDIV = 1
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;
    //HSPCLKDIV = 1
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
    //��������ģʽ
    EPwm3Regs.TBCTL.bit.CTRMODE = 2;
    //����������0���øߵ�ƽ������ֵ����PRD���õ͵�ƽ
    EPwm3Regs.AQCTLA.all = 0x0006;
    //1khz PWM
    EPwm3Regs.TBPRD = 90000 /2;
    //%50ռ�ձ�
    EPwm3Regs.CMPA.half.CMPA = 90000/4;
    #endif
    /***************************/

    #ifdef PWM_PHASE_TEST
    //��CTRΪ0ʱ��ͬ�����
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;
    //ʹ��EPWM2����
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    //����ͬ����λ
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;
    //��λ1/3����120��
    EPwm2Regs.TBPHS.half.TBPHS = 90000 / 2 /3;
    //ʹ��EPWM3����
    EPwm3Regs.TBCTL.bit.PHSEN = 1;
    //��λ240��
    EPwm3Regs.TBPHS.half.TBPHS = 90000 / 3;
    #endif

    #ifdef PWM_DEAD_BAND_TEST
    EPwm1Regs.DBRED = 750;              // ��������ʱ10 ms
    EPwm1Regs.DBFED = 750;              // �½�����ʱ10 ms
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;   // ePWM1A = RED
    EPwm1Regs.DBCTL.bit.POLSEL = 2;     // S3=1 ePWM1B�����ź�
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;    // ePWM1A ��Ϊ�����ź�
    #endif

    #ifdef PWM_CHOPPER_TEST
    EPwm1Regs.PCCTL.bit.CHPFREQ = 7;   // SYSCLKOUT / 64  = 2.34375MHz
    EPwm1Regs.PCCTL.bit.CHPDUTY = 3;    // 50% ռ�ձ�
    EPwm1Regs.PCCTL.bit.CHPEN = 1;      // ʹ��ն��ģʽ
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


