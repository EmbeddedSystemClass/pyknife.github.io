#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Buzzer
// Timer1 is used for buzzing

#include "common.h"



static byte buzzLen;
static millis8_t startTime;
static buzzFinish_f onFinish;
static tonePrio_t prio;

static void stop(void);



//TIM13 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM13_PWM_Init_Init(u32 arr,u32 psc)
{                                                
       //�˲������ֶ��޸�IO������
       
       GPIO_InitTypeDef GPIO_InitStructure;
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruc;
       TIM_OCInitTypeDef  TIM_OCInitStruc;
       
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);        //TIM13ʱ��ʹ��   
       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);        //ʹ��PORTFʱ��       
       
       GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13); //GPIOF8����Ϊ��ʱ��13
       
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;         //GPIOF8
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //�ٶ�100MHz
       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //���츴�����
       GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //����
       GPIO_Init(GPIOF,&GPIO_InitStructure);          //��ʼ��PF8
         
       TIM_TimeBaseStruc.TIM_Prescaler=psc;  //��ʱ����Ƶ
       TIM_TimeBaseStruc.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
       TIM_TimeBaseStruc.TIM_Period=arr;  //�Զ���װ��ֵ
       TIM_TimeBaseStruc.TIM_ClockDivision=TIM_CKD_DIV1; 
       
       TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStruc);//��ʼ����ʱ��13
       //��ʼ��TIM13 Channel1 PWMģʽ       
       TIM_OCInitStruc.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
       TIM_OCInitStruc.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
       TIM_OCInitStruc.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
       TIM_OCInitStruc.TIM_Pulse=0;
       TIM_OC1Init(TIM13, &TIM_OCInitStruc);  //����Tָ���Ĳ�����ʼ������TIM13 OC1

       TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //ʹ��TIM13��CCR1�ϵ�Ԥװ�ؼĴ���

       TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPEʹ�� 
       
       TIM_Cmd(TIM13, ENABLE);  //ʹ��TIM13                                                                    
}  



//��������ʼ��
//��ʼ��PF8Ϊ�����		    
//BEEP IO��ʼ��
void buzzer_init()
{
	TIM13_PWM_Init_Init(14399,10);
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ�� 
  //��ʼ����������Ӧ����GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
	
  GPIO_ResetBits(GPIOF,GPIO_Pin_8);  //��������Ӧ����GPIOF8���ͣ� 

}

// Non-blocking buzz
void buzzer_buzz(byte len, tone_t tone, vol_t volType, tonePrio_t _prio, buzzFinish_f _onFinish)
{
	if(_prio < prio)
		return;
	else if(tone == TONE_STOP)
	{
		stop();
		return;
	}


	
	// Tell power manager that we are busy buzzing
//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_IDLE);

	prio = _prio;
	onFinish = _onFinish;
	buzzLen = len;
	startTime = millis();

	// Silent pause tone   //������ͣ��
	if(tone == TONE_PAUSE)
	{
//		CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//		power_timer1_disable();
    TIM_SetCompare1(TIM13,0);
	  BEEP=0; //PB.5 ����� 
    TIM_Cmd(TIM13, DISABLE);  //ֹͣTIM3 

		return;
	}

	// Workout volume
	uint ocr;
	byte vol;

	switch(volType)
	{
//		case VOL_UI:
//			vol = appConfig.volUI;
//			break;
//		case VOL_ALARM:
//			vol = appConfig.volAlarm;
//			break;
//		case VOL_HOUR:
//			vol = appConfig.volHour;
//			break;
		default:
			vol = 2;
			break;
	}

	// Pulse width goes down as freq goes up
	// This keeps power consumption the same for all frequencies, but volume goes down as freq goes up

	vol--;
	if(vol > 2)
		return;

  uint icr = tone * (8 << 1);

	ocr = icr - (icr / (32>>vol));

	   TIM_Cmd(TIM13, ENABLE);  //��ʼTIM3 
//	TIFR1 = 0;
//	SET_BITS(TIMSK1, TOIE1);
		  BEEP=1; //PB.5 ����� 
    TIM_SetCompare1(TIM13,ocr);
//	TCNT1 = 0;
//	OCR1A = ocr;
//	ICR1 = icr;
	//SET_BITS(TCCR1A, COM1A1, COM1A0);
}
/*
#include "led.h"

void buzzer_buzzb(byte len, tone_t tone, vol_t volType)
{
	(void)(volType);

	led_flash(LED_GREEN, 50, 255);
	led_flash(LED_RED, 50, 255);

	power_timer1_enable();
	TCCR1A |= _BV(COM1A1)|_BV(COM1A0);

//	static uint vol = 0;
//	vol++;
//	if(vol > 790)
//		vol = 1;
//	OCR1A = vol;

//	if(vol > 3)
//		vol = 0;
//	if(vol == 0)
//		OCR1A = tone; // normal
//	else if(vol == 1)
//		OCR1A = (tone * 2) - 50; // quiet
//	else if(vol == 2)
//		OCR1A = (tone / 2); // loud
//	else if(vol == 3)
//		OCR1A = (tone / 4); // loader (acually quiter)

	OCR1A = (tone * 2) - 100;
	ICR1 = tone * 2;
	while(len--)
	{
		delay(1);
		led_update();
	}		
//	delay(20);
	TCCR1A &= ~(_BV(COM1A1)|_BV(COM1A0));
	power_timer1_disable();
}
*/
// Are we buzzing?
bool buzzer_buzzing()
{
	return buzzLen;
}

// See if its time to stop buzzing
void buzzer_update()
{
	if(buzzLen && (millis8_t)(millis() - startTime) >= buzzLen)
	{
		stop();

		if(onFinish != NULL)
			onFinish();
	}
}

static void stop()
{
//	CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//	power_timer1_disable();
    TIM_SetCompare1(TIM13,0);
	  BEEP=0; //PB.5 ����� 
    TIM_Cmd(TIM13, DISABLE);  //ֹͣTIM3 
	
	buzzLen = 0;
	prio = PRIO_MIN;

//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_NONE);
}

// Sometimes the buzzer kind of 'pops' from a bad waveform output (the first HIGH pulse is too long)
// Here we wait until a timer overflow and then turn on the timer output
// It still sometimes pops, but much less so than turning on the timer output in buzzer_buzz()
//ISR(TIMER1_OVF_vect)
//{
//	SET_BITS(TCCR1A, COM1A1, COM1A0);
//	TIMSK1 = 0;
//}






