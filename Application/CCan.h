/*
 * CCan.h
 *
 *  Created on: 2023��2��18��
 *      Author: 97478
 */

#ifndef APPLICATION_CCAN_H_
#define APPLICATION_CCAN_H_

void initCanGpio();
void initCan();
void run();
void CAN_Task(void * pvParameters);


#endif /* APPLICATION_CCAN_H_ */
