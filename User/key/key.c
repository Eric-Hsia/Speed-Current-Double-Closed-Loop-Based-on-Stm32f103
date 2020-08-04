#include "key.h"
#include "delay.h"

typedef struct 
{
	GPIO_TypeDef* GPIOX;
	uint16_t GPIO_Pin;
}KGPT[2][4]; //4*4
KGPT KPIN={ //ֻ��Ҫ��������д�����е�IO�ں͹ܽ�   IO�ں͹ܽŶ��������ⶨ�� ������һ��IO�� �ܽ�Ҳ���谴˳��
						{{GPIOE,GPIO_Pin_0} ,{GPIOE,GPIO_Pin_1}, {GPIOE,GPIO_Pin_2},{GPIOE,GPIO_Pin_3}},//row ����
						{{GPIOE,GPIO_Pin_4},{GPIOE,GPIO_Pin_5}, {GPIOE,GPIO_Pin_6},{GPIOE,GPIO_Pin_7}}  //col ����
					};
#define row 0
#define col 1					
#define RCC_APB2_GPIOX  RCC_APB2Periph_GPIOE//|RCC_APB2Periph_GPIOC//IOʱ��					
#define MAX_Time   36000000 // ���ּ�������ʱ   
//������ʼ������ 
void KEY_Init(void)
{
  u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2_GPIOX,ENABLE);//ʹ��PORTCʱ��
  for(i=0;i<4;i++)
	
	{
	 GPIO_InitStructure.GPIO_Pin = KPIN[row][i].GPIO_Pin;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	 GPIO_Init(KPIN[row][i].GPIOX, &GPIO_InitStructure);//��ʼ��row
	 GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);
	}
 for(i=0;i<4;i++)
	{
		 GPIO_InitStructure.GPIO_Pin  = KPIN[col][i].GPIO_Pin;//PC5
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
		 GPIO_Init(KPIN[col][i].GPIOX, &GPIO_InitStructure);//��ʼ��col
		GPIO_SetBits(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin);
	}
} 

u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex)
{
 u8 a;
 delay_ms(1);//������ʱ
 for(a = 0;a<4;a++){
  GPIO_SetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin);//ÿ������1
  if((GPIO_ReadInputDataBit(GPIOX, colPin) == 1)) //�����Ҳ���� �е�ֵ��֪���� Ϊ i
		{
			GPIO_ResetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin); //�лָ� ��0 
		  return colIndex+a*4+1;//���ص����� Ϊ1-16 ��Ӧ4x4���̵�16����
		}
	GPIO_ResetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin);     //�лָ� ��0
 }
 return 0;
}
u8 KEY_Scan(void)
{	 
 u8 i,keyvalue,j;
	//u32 j=0;
	for(i = 0;i<4;i++)
	{
	 if(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)//����� ��ֵΪ i
	 {
			keyvalue = ReScan(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin,i);//����� ȡ��ֵ
		 for(j=0;j<4;j++)
		 {
			 GPIO_SetBits(KPIN[col][j].GPIOX,KPIN[col][j].GPIO_Pin);
		 }
		 for(j=0;j<4;j++)
		 {
			 GPIO_ResetBits(KPIN[row][j].GPIOX,KPIN[row][j].GPIO_Pin);
		 }
		  while(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0);
		//	while((GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)&&(j<MAX_Time))j++;//���ּ��
			return keyvalue;//���ؼ�ֵ
		}
	}
return 0;
}


					


