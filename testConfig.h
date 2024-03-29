/*
 * testConfig.h
 *
 *  Created on: 2023年2月19日
 *      Author: 97478
 */

#ifndef TESTCONFIG_H_
#define TESTCONFIG_H_

//#define CAN_TEST                //CAN测试任务开关
#define PWM_TEST                //PWM测试任务开关
#define PWM1_TEST               //PWM1 输出
//#define PWM1_NORMAL_TEST        //PWM1 占空比50测试
#define PWM1_PULSE_MUTABLE_TEST  //PWM1 脉宽变化（0-100%）
#define PWM1_A_B_COMPLEMENTARY   //PWM1 A B互补信号
#define PWM2_TEST               //PWM2 输出
#define PWM3_TEST               //PWM3 输出
#define PWM_PHASE_TEST          //PWM 3个输出且相位差120°
#define PWM_DEAD_BAND_TEST      //PWM1 AB死区输出，需要打开PWM1_TEST
#define PWM_CHOPPER_TEST        //PWM1 AB互补，斩波输出，需要打开PWM1_TEST




#endif /* TESTCONFIG_H_ */
