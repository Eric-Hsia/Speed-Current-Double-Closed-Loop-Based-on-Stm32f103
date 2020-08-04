/**
  ******************************************************************************
  * @file    main.c
  * @author  LYC
  * @version V1.0
  * @date    2014-04-22
  * @brief   ��ֲҰ������I2C
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "balance.h"
#include "STM32F1.h"
#include "bsp_usart1.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_pwm_output.h" 
void TIM2_NVIC_Config(void);
void TIM2_Config(void);
void TIM2_IRQHandler(void);
static int count=0;
int canshuOK_p=0;
int canshuOK_i=0;
int qidongOK=0;
int canshuOK_ip=0;
int canshuOK_iI=0;
uint8_t color=0;
extern float SpeedSet;
extern float Speed_control_p;
extern float SPEED_CONTROL_I;
extern float Current_control_p;
extern float Current_CONTROL_I;
//extern short Accel[3];
//extern	short Gyro[3];
extern float gf_nowspeed,
	           gf_GPT,
             gf_SpeedControlIntegral,
             gf_SpeedControlOut,
             gf_current;
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
/* LED���ȵȼ� PWM�� */
uint16_t indexWave[] = {998,998,997,996,995,993,990,986,981,973,961,944,921,886,837,766,663,516,305,0,0,305,516,663,
766,837,886,921,944,961,973,981,986,990,993,995,996,997,998,998};
float var[2];
int i;
int speed_count=0;
int tiaoshi=0;
int tiaoshi_ok=0;
int main(void)
{
	//uint8_t datatemp[ SIZE ];
	delay_init();//�δ���ʱ��ʼ��
	delay_ms(5);
	KEY_Init();  //������̳�ʼ��
	GPIO_Configuration();//lcd�˿ڳ�ʼ��
	LCD1602_Init();      //lcdʹ��
	//I2C��ʼ��
	//i2c_GPIO_Config();
  //MPU6050��ʼ��
	//MPU6050_Init();
	
	STMFLASH_Read ( FLASH_SAVE_ADDR, ( uint16_t * ) TEXT_Buffer, SIZE );
	delay_ms(5);
	LCD1602_Show_Str(0,0,"Debug");
	while(tiaoshi_ok==0)
	{
		switch (KEY_Scan())
		{
			case 1:
				tiaoshi=0;
			  LCD1602_ClearScreen();
			  LCD1602_Show_Str(0,0,"Debug");
			break;
			case 2:
				tiaoshi=1;
			  LCD1602_ClearScreen();
			  LCD1602_Show_Str(0,0,"Release");
			break;
			case 3:
				tiaoshi_ok=1;
			break;
		}
	}
	LCD1602_ClearScreen();
	if(tiaoshi==1)
	{
		LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		while(KEY_Scan()==0);
		LCD1602_ClearScreen();
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(KEY_Scan()==0); 
		//USART_Cmd(macUSARTx, DISABLE);
		USARTx_Config_2();//��ʼ������2��Զ�̿���
	}
	else if(tiaoshi==0)
{
	USARTx_Config();//����1ͨ�ų�ʼ��
	//USART_Cmd(macUSARTx_2, DISABLE);
	LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);//��ʾ�ٶȻ�����ϵ��
	//�����޸ı���ϵ��
	while(canshuOK_p==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]+=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]-=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 3:
			canshuOK_p=1;
		  
		break;
		
		
	}
		i=0;
	}
	canshuOK_p=0;
	LCD1602_ClearScreen();
	//�����޸�ת�ٻ�����ϵ��
	LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
	while(canshuOK_i==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]+=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]-=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 3:
			canshuOK_i=1;
		  
		break;
		
		
	}
	i=0;
	}
	canshuOK_i=0;
	LCD1602_ClearScreen();
	//�����޸ĵ���������ϵ��
	LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
	while(canshuOK_ip==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]+=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]-=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 3:
			canshuOK_ip=1;
		  
		break;
		
		
	}
	i=0;
	}
	canshuOK_ip=0;
	LCD1602_ClearScreen();
	//�����޸ĵ���������ϵ��
	LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(canshuOK_iI==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]+=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]-=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 3:
			canshuOK_iI=1;
		  
		break;
		
		
	}
	i=0;
	}
}
  canshuOK_iI=0;
	LCD1602_ClearScreen();
//������������
	LCD1602_Show_Str(0,0,"Start now?");
	while(qidongOK==0)
	{
		i=KEY_Scan();
		if(i==3)
			qidongOK=1;
		i=0;
	}		
	qidongOK=0;
	LCD1602_ClearScreen();

	
	LED_GPIO_Config();//�������ת����
	
  TIMx_PWM_Init();//pwm��ʼ��TIM3
	Adc_Init();//ad�ɼ���ʼ��������
	TIM4_Init();//��������ʼ��TIM4
	TIM2_NVIC_Config();//��ʱ�жϳ�ʼ������ʱ����
	TIM2_Config();

while(1)
{
	if(tiaoshi==0)
	{
//������ʾת�ٵ���
	var[0]=gf_nowspeed;
	var[1]=gf_current;
	vcan_sendware(var, sizeof(var));
	//delay_us(500);
	delay_ms(1);
	}
	//������������ת��
	i=KEY_Scan();
	switch(i)
	{
		case 1:
			SpeedSet+=50;
		break;
		case 2:
			SpeedSet-=50;
		break;
		case 3:
			SpeedSet=0;
		break;
		case 4:
			SpeedSet=-SpeedSet;
		break;
		case 5:
			TIM_Cmd(TIM2,DISABLE);
		  LED1_OFF;
	    LED2_OFF; 
		  tiaoshi_ok=0;
		  STMFLASH_Write ( FLASH_SAVE_ADDR, ( uint16_t * ) TEXT_Buffer, SIZE );
		  LCD1602_ClearScreen();
		  LCD1602_Show_Str(0,0,"Parameter saved");
		break;
		case 6:
		{
			TIM_Cmd(TIM2,DISABLE);
		  LED1_OFF;
	    LED2_OFF; 
		  LCD1602_ClearScreen();
		 	LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);//��ʾ�ٶȻ�����ϵ��
	//�����޸ı���ϵ��
	while(canshuOK_p==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]+=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]-=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 3:
			canshuOK_p=1;
		  
		break;
		
		
	}
		
	}
	canshuOK_p=0;
	LCD1602_ClearScreen();
	//�����޸�ת�ٻ�����ϵ��
	LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
	while(canshuOK_i==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]+=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]-=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 3:
			canshuOK_i=1;
		  
		break;
		
		
	}

	}
	canshuOK_i=0;
	LCD1602_ClearScreen();
	//�����޸ĵ���������ϵ��
	LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
	while(canshuOK_ip==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]+=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]-=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 3:
			canshuOK_ip=1;
		  
		break;
		
		
	}

	}
	canshuOK_ip=0;
	LCD1602_ClearScreen();
	//�����޸ĵ���������ϵ��
	LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(canshuOK_iI==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]+=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]-=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 3:
			canshuOK_iI=1;
		  
		break;
		
		
	}

	}
  canshuOK_iI=0;
	LCD1602_ClearScreen();
	LCD1602_Show_Str(0,0,"Start now?");
	while(qidongOK==0)
	{
		
		if(KEY_Scan()==3)
			qidongOK=1;
	}		
	qidongOK=0;
	LCD1602_ClearScreen();
	TIM_Cmd(TIM2,ENABLE);
  }
		break;
	}
	i=0;
	if(tiaoshi_ok==1)
	{
	//LCD��ʾ�趨�뵱ǰ�ٶ�
	LCDshowNum(0,0,"SpeedSet:",SpeedSet);
	LCDshowNum(0,1,"Now speed:",gf_nowspeed);
		
	}
}

}
void TIM2_NVIC_Config(void)//��ʱ�ж����ȼ�����
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void TIM2_Config(void)//��ʱ�ж�ʱ������
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler=71;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=2000;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
void TIM2_IRQHandler(void)
{
	static uint8_t pwm_index = 0;			//����PWM���
	static uint8_t period_cnt = 0;		//���ڼ���������
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		 count++;
		 period_cnt++;
		if(period_cnt >= 50)										//�����������������10�������һ��������PWM��
			{
				switch (color)
				{
				case 0:
				vMotoXPwmOutright(indexWave[pwm_index]);
				vMotoXPwmOutleftright(indexWave[pwm_index]);
				break;
				case 1:
					vMotoXPwmOutright(indexWave[pwm_index]);
					vMotoXPwmOutleftright(998);
				break;
				case 2:
					vMotoXPwmOutright(998);
					vMotoXPwmOutleftright(indexWave[pwm_index]);
				break;
				}
				pwm_index++;												//��־PWM�����һ��Ԫ��
			
				if( pwm_index >=  40)								//��PWM������Ѿ�������һ�飬����PWM����־
				{
					pwm_index=0;	
          color++;
          if(color>=3)
						color=0;
				}
				
				period_cnt=0;												//�������ڼ�����־
			}
  switch (count)
  {
  case 1:
    
	  speed_count++;
    GetMoterpluse();//4ms�ɼ�һ������
	if(speed_count==10)//40ms����һ���ٶ�pid
	{
	  speedpid();
		speed_count=0;
	}
    break;
 
    
    case 2:
		currentpid();//4ms�ɼ�һ�ε���ֵ�����е���pid
		Moteroutput();//���Ƶ��
    count=0;
    break;
  }
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
	}
}


/*********************************************END OF FILE**********************/
