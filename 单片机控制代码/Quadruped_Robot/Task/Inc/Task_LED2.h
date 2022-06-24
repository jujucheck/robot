/*
 * @Description: CANͨѶͷ�ļ�����
 * @Author: Mr Zhang
 * @Github: https://github.com/bigfatpaper
 * @Date: 2019-10-13 14:00:36
 * @LastEditors: Mr Zhang
 * @LastEditTime: 2019-10-16 23:58:33
 */

/*����Ԥ����*/
#ifndef __TASKS_CAN_H
#define __TASKS_CAN_H

/*�ⲿ����Ԥ����*/
#ifdef __TASK_CAN_GLOBALS
#define TASK_CAN_EXT
#else
#define TASK_CAN_EXT extern
#endif
/*�ⲿ����Ԥ�������*/

/*ͷ�ļ�*/
#include "Task_init.h"
/*����ͷ�ļ�����*/

/*�궨��*/
#define CANSEND_1 1                        /*CANSEND���� 1 0x200 2 0x1ff*/
#define CANSEND_2 2
/*�궨�����*/

/*�ṹ�嶨��*/

typedef struct                             /*CAN���ͽṹ��*/
{
    uint8_t            CANx;               /* CAN1 or CAN2 ����*/
    uint32_t           stdid;              /* ID: CAN1 0X200  CAN2 0X1FF */
		uint8_t            Data[8];            /* CAN��������*/
}CanSend_Type;

/*�ṹ�嶨�����*/

/*��������*/
TASK_CAN_EXT CanSend_Type CAN_Tx_Msg;      /*CAN���ͱ�������*/
/*������������*/

/*��������*/
/*CAN�ĳ�ʼ������ �˲���ʹ���ж�*/
void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_Recieve(CAN_HandleTypeDef *hcan);

extern int16_t delta_Mechanical_angle(int16_t speed, uint16_t angle_last, uint16_t angle_now);
/*������������*/

#endif
/*����Ԥ�������*/
