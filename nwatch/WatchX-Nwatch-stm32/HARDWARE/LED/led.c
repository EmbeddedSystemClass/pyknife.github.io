#include "led.h" 
#include "common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


/*
   Project: N|Watch
   Author: Zak Kemble, contact@zakkemble.co.uk
   Copyright: (C) 2013 by Zak Kemble
   License: GNU GPL v3 (see License.txt)
   Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/

// LED control

typedef struct {
  byte flashLen;			// How long to light up for
  millis8_t startTime;	//
} led_s;

static led_s ledRed;
static led_s ledGreen;


//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void led_init()
{
  // Setup timers
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����
}

//static void flash(led_s* led, byte len, byte brightness, volatile byte* ccr, volatile byte* ocr, byte com, volatile byte* port, byte pin)
static void flash(led_s* led, byte len,byte pin)
{
  led->flashLen = len;
  led->startTime = millis();

//  if (brightness == 255 || brightness == 0)
//  {
//    *ccr &= ~com;
//    brightness == 255 ? (*port |= pin) : (*port &= ~pin);
//  }
//  else
//  {
////#ifdef __AVR_ATmega32U4__
////    power_timer4_enable();
////#else
////    power_timer0_enable();
////#endif
//    *ccr |= com;
//    *ocr = brightness;
//  }
}

void led_flash(led_t led, byte len, byte brightness)
{
//  if (!appConfig.CTRL_LEDs) 
	{

    switch (led)
    {
      case LED_RED:
//        flash(&ledRed, len, brightness, &RED_CCR, &RED_OCR, _BV(RED_COM), &RED_PORT, _BV(RED_PIN));
			flash(&ledRed, len, 9);
        break;
      case LED_GREEN:
//        flash(&ledGreen, len, brightness, &GREEN_CCR, &GREEN_OCR, _BV(GREEN_COM), &GREEN_PORT, _BV(GREEN_PIN));
			flash(&ledGreen, len, 10);
        break;
      default:
        break;
    }

//    pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_IDLE);
  }
}

// Is an LED on?
BOOL led_flashing()
{
  return ledRed.flashLen || ledGreen.flashLen;
}

//static BOOL _update(led_s* led, volatile byte* ccr, byte com, volatile byte* port, byte pin)
static BOOL _update(led_s* led,  byte pin)        //(volatile unsigned long  *)
{
  if (led->flashLen && (millis8_t)(millis() - led->startTime) >= led->flashLen)
  {
   PFout(pin)=0;
    led->flashLen = 0;
  }
	else
		 PFout(pin)=1;


  return led->flashLen;
}

void led_update()
{
//  BOOL red = _update(&ledRed, &RED_CCR, _BV(RED_COM), &RED_PORT, _BV(RED_PIN));
//  BOOL green = _update(&ledGreen, &GREEN_CCR, _BV(GREEN_COM), &GREEN_PORT, _BV(GREEN_PIN));
BOOL red = _update(&ledRed,9);          //LED0 PFout(9)
BOOL green = _update(&ledGreen, 10);    //LED1 PFout(10)

	
//  if (!red && !green)
//  {
    // Timer no longer in use, disable
//#ifdef __AVR_ATmega32U4__
//    power_timer4_disable();
//#else
//    power_timer0_disable();
//#endif
//    pwrmgr_setState(PWR_ACTIVE_LED, PWR_STATE_NONE);
//  }
}

// Turn off LEDs
void led_stop()
{
  led_flash(LED_GREEN, 0, 0);
  led_flash(LED_RED, 0, 0);
  led_update();
}



