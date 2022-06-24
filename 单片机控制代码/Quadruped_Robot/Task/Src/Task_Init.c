/*ͷ�ļ�Ԥ����*/
#define __TASK_INIT_GLOBALS
/*ͷ�ļ�Ԥ�������*/

/*����ͷ�ļ�*/
#include "Task_init.h"
#include "tim.h"
//#include "MPU6050.h"

/* -------------------------------------------------------------------------- */
static void Servo_Init(void);

/* ---------------------------- Static Variables ---------------------------- */


/* ---------------------------- global functions ---------------------------- */

/**
  * @brief  Create all task
  * @note   
  * @param  parameters: none
  * @retval None
  */
void Task_Init_Start(void *parameters)
{
	taskENTER_CRITICAL();           //��ʼ�������н�ֹ�����������
	
	//IMU_Init();                   //�����ǳ�ʼ��
	Servo_Init();                   //PWM��ʼ��
  printf("** Peripheral inited successfully. ** \r\n");
	
	HAL_Delay(2000);                //��ʱ�������������
	vTaskDelete(NULL);              //ɾ����ʼ��������
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

/* ----------------------- Static Function Definitions ---------------------- */


void Servo_Init(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);	
}
