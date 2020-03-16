#include "ADC.h"

#define ADC1_DR_Address ( (u32)0x4001244C)

__IO u16 ADC_ConvertedValue[ADC_CHANEL_NUM];

/*��ʼ��ADC1 */
void ADC1_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	pinMode(GPIOA, GPIO_Pin_2, GPIO_Mode_AIN);
	pinMode(GPIOA, GPIO_Pin_3, GPIO_Mode_AIN);
	pinMode(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN);
	pinMode(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN);
	pinMode(GPIOA, GPIO_Pin_6, GPIO_Mode_AIN);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                              /* ʹ��MDA1ʱ�� */
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);                                                      /* ָ��DMAͨ�� */
	DMA_InitStructure.DMA_PeripheralBaseAddr	= ADC1_DR_Address;              /* ����DMA�����ַ */
	DMA_InitStructure.DMA_MemoryBaseAddr		= (u32) & ADC_ConvertedValue;   /* ����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ */
	DMA_InitStructure.DMA_DIR			= DMA_DIR_PeripheralSRC;        /* ����Ϊ����Ϊ���ݴ������Դ */
	DMA_InitStructure.DMA_BufferSize		= ADC_CHANEL_NUM;               /* DMA����������Ϊ1�� */
	DMA_InitStructure.DMA_PeripheralInc		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode			= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority			= DMA_Priority_High;
	DMA_InitStructure.DMA_M2M			= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);                                                 /* ʹ��DMAͨ�� */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                            /* ʹ��ADC1ʱ�� */

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode			= ADC_Mode_Independent;         /* ʹ�ö���ģʽ��ɨ��ģʽ */
	ADC_InitStructure.ADC_ScanConvMode		= ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;                       /* ������Ӵ����� */
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;    /* ʹ�������Ҷ��� */
	ADC_InitStructure.ADC_DataAlign			= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel		= ADC_CHANEL_NUM;               /* ֻ��1��ת��ͨ�� */
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel11 configuration */
  /* ͨ��1��������55.5��ʱ������ */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);       /* ʹ��ADC��DMA */

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);          /* ʹ��ADC1 */

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1) )
	{
		;
	}

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1) )
	{
		;
	}

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); /* ��ʼת�� */
}

