#include "stm32f10x.h"
#include "myInclude.h"
#include "Robot.h"

#define FAILSAFE

int main(void)
{
	double tmp;
	/****************ϵͳ��ʼ��****************/
	SysTick_Init();			/* SysTick��ʼ��������Ϊϵͳʱ�� */
	USART1_Init();			/* ���ڳ�ʼ�������ڵ��� */

	/****************������ʼ��****************/
	TIM3_PWM_Init();        /* PWM��ʼ����ʹ��TIM3����20KHzƵ����ͨ��PWM */
	ADC1_Init();            /* �����Ǽ��ٶȼƳ�ʼ����ʹ��DMA����ģʽ��6ͨ����������������55.5��ʱ������ */
	Encoder1_Init();        /* ������1��ʼ������TIM2����Ϊ�������빦�� */
	Encoder2_Init();        /* ������2��ʼ������TIM4����Ϊ�������빦�� */
	Timer1_Init(5);			/* ��TIM1����Ϊ5ms��ʱ�жϣ����������п����㷨 */

	/****************�����㷨��������****************/
	CTRL_AngleX.q1	= 300, CTRL_AngleX.q3	= 5, CTRL_SpeedX.q1	= 0.05, CTRL_SpeedX.q2	= 0.8;  
	CTRL_AngleY.q1	= 400, CTRL_AngleY.q2	= 5.5, CTRL_SpeedY.q1	= 0.011, CTRL_SpeedY.q3	= 0.08; 

	/****************׼������****************/
	while(millis() < 3000);
	
	CTRL_AngleY.Setpoint	= compAngleY;
	RELAX_ANGLEY		= compAngleY;
	FLAG |= A_START; 
	CTRL_AngleX.Setpoint	= compAngleX;
	RELAX_ANGLEX		= compAngleX;

	FLAG |= EC_START;
            
	/****************��ѭ������****************/
	while(1)
	{  

	}
}

