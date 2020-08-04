#include "KeyScan.h"


void Delay_ms1(int xms)	
{
  u32 i,j;
	for(i=xms;i>0;i--)
	  for(j=72000;j>0;j--);
}
void Scan_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);							//������Ϊ�ߵ�ƽ
	GPIO_ResetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7);						//������Ϊ�͵�ƽ
}




 
u8 Read_KeyValue(void)
{
	u8 KeyValue=0;
	if((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F)								//ȡGPIOC����λ��ƽֵ���жϰ����Ƿ���
	{
		Delay_ms1(8);																					//��ʱ����������ʱ�����������ʱ����׼ȷ��ʱ
		if((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F)						//�ٴ��жϰ����Ƿ���
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);										//���õ�һ��Ϊ �ߵ�ƽ��������Ϊ�͵�ƽ��ͨ��ɨ�裬�����ж���һ�г��ָߵ�ƽ
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);				
			switch(GPIO_ReadInputData(GPIOE)&0xFF)							//ȡGPIOC����λ��ƽֵ���ж���һ��������
			{
				case 0x11: KeyValue = 1; break;				// 0001 0001 
				case 0x21: KeyValue = 5; break;				// 0010 0001
				case 0x41: KeyValue = 9; break;				// 0100 0001
				case 0x81: KeyValue = 13; break;				// 1000 0001
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_2|GPIO_Pin_3);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x12: KeyValue = 2; break;   //0001 0010
				case 0x22: KeyValue = 6; break;   //0010 0010
				case 0x42: KeyValue = 10;break;    //0100 0010
				case 0x82: KeyValue = 14;break;    //1000 0010
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_3);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x14: KeyValue = 3; break;   //0001 0100
				case 0x24: KeyValue = 7; break;   //0010 0100
				case 0x44: KeyValue = 11; break;   //0100 0100
				case 0x84: KeyValue = 15; break;   //1000 0100
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x18: KeyValue = 4; break;   //0001 1000
				case 0x28: KeyValue = 8; break;   //0010 1000
				case 0x48: KeyValue = 12; break;   //0100 1000
				case 0x88: KeyValue = 16; break;   //1000 1000
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7);
			
			while((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F);				//�ȴ������ɿ�
			return KeyValue;
			//printf("���µļ�ֵ:%d\n",KeyValue);
			}
	}
	return 0;
}

