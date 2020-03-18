#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "common.h"
//���������� keil5  ALIENTEK ̽����STM32F407������  2.4��tft��Ļ
//
//Nwatch�Ѿ��ɹ���ֲ����Stm32����Ŀǰ�Ѿ���ɵ���ֲ����Ϊ��
//                                                     RTCʱ�ӡ�������ʾ���ϵ�flashģ��eeprom�������Դ�������ͼ��
//                                 ����ɣ�             �����flash�洢��Sleep˯��ģʽ���͹���ģʽ������ѹ���
//
//��ʵ�ֵ�Ч�������ӡ��ֵ�Ͳ������ʱ��3D������ʾ����Ϸ���������ƻ�С��
//             ����->���ڵ�������˯��ʱ�����ã�����ʾ���á��汾��Ϣ�鿴��FPS��ʾ
//����Ȥ��С�����Խ�Ⱥ			
//                                                  
//��Ҫ�Ĳ˵���������Ķ������setting.c �ĵ���������  static void itemLoader(byte num)   ͬʱע�� #define OPTION_COUNT	5�м��OPTION_COUNT ����ʱ������������Ӽ���
//                          void setMenuOption_P(byte num, const char* name, const byte* icon, menu_f actionFunc)  �е�actionFunc  ���������Go to Definition  of "xxxxx" �Ϳ��Խ����Ӧ���ļ������޸ģ�Ҳ����ģ�½�������ļ�
//                        
//����һ�ֲ˵�ģʽ�����Խ�� timedate.c �� diag.c
//
//�����뽫��Ļ��ֲ��oled ��С��飬ֻ��Ҫ��void oled_flush(void)��д��oledBuffer[] �����oled��Ļ��
//����Ϊ���ϻ�û��oled ��ֻ��ģ����oled ����ʾ��ʽ�������޸�����λ��
//
//��������ֲ������stm32��Ƭ���ϵ�С��飬��Ҫ�ر�ע��appconfig.c�м��eepCheck_SAVE_ADDR ���ƫ�Ƶ�ַ�����ʵ�����
// EEPROM_CHECK_NUM���ֵ����ѧ���ҳ����˺ܾòų���
//
//��ֲ�͸Ķ��ļ������ʱ����Ҫ��Ҫ��   common.h
//����Ԥ���벻����Ŀ����޸ģ�        config.h
//
//������ʾ��ɧ������ lcd.c LCD_Fast_DrawPoint ������һ�������������
//
//�������lcd.c���Ҽ򻯹�����ԭ�ӵĿ�ģ������Լ�����Ļ��Ҫ���Ҹ������õ���Ļ���޸�һ��
//
//ϣ����������ڼ���Ŀ��ģ��������壡����
//
//                                                   ���ߣ�YUMMY

void c_setup()
{
  SystemInit();	
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);          //��ʼ����ʱ����
#if COMPILE_UART
		uart_init(UART_BAUD);		//��ʼ�����ڲ�����Ϊ115200
#endif
 	LCD_Init();               //��ʼ��LCD FSMC�ӿ�
	POINT_COLOR=BLACK;        //������ɫ����ɫ
  // Everything else
	memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
	led_init();               //��ʼ��LED
  buzzer_init();	
  global_init();
	appconfig_init();
	alarm_init();             //�޷��������ӣ�ÿ�������Ժ���Ҫ�Զ���

  buttons_init();
  millis_init();
  time_init();
	rtc_init();
	LED0=!LED0;
	
	// Set watchface
	display_set(watchface_normal);

	display_load();
	
	//���ñ�����ɫ
  if(	appConfig.invert)
	  LCD_Clear(WHITE);
  else   	
	  LCD_Clear(BLACK);
}

void c_loop()
{
    time_update();
//  if(pwrmgr_userActive())
//  {
//    battery_update();
//    buttons_update();
//  }
    
	  buttons_update();
  	buzzer_update();
    led_update();
	
	
#if COMPILE_STOPWATCH
  stopwatch_update();
#endif
//  global_update();

//  if(pwrmgr_userActive())
//  {
    alarm_update();
    display_update();
//  }

  // freeRAM();
//  wdt_update();
//  pwrmgr_update();

//��ʾ��ɺ����������
	  memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);

}



int main(void)
{ 
	
	c_setup();	    //��ʼ��

  while(1)
		{
	     c_loop(); //ѭ��
	  }

}





