/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��usart2.c
 * ����    ����printf�����ض���USART2�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨������STM32������
 * Ӳ�����ӣ�---------------------
 *          | 									  |
 *          | PA2  - USART2(Tx)   |
 *          | PA3  - USART2(Rx)   |
 *          |                     |
 *           ---------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "bsp_usart2.h"
#include <stdarg.h>
uint8_t  Uart_Rx[UART_RX_LEN ]={0};

/// ����USART2�����ж�
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = macUSART_IRQ_2;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������USARTx_Config
 * ����  ��USART2 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USARTx_Config_2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	/* config USART2 clock */
	macUSART_APBxClock_FUN_2(macUSART_CLK_2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel5);
	macUSART_GPIO_APBxClock_FUN_2(macUSART_GPIO_CLK_2, ENABLE);

	/* USART2 GPIO config */
	 /* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(macUSART_TX_PORT_2, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(macUSART_RX_PORT_2, &GPIO_InitStructure);
	 ///////////////////// 
		DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( macUSARTx_2->DR ) );	
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart_Rx;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = UART_RX_LEN   ;

		/*�����ַ����*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*�ڴ��ַ����*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*�������ݵ�λ*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*�ڴ����ݵ�λ 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMAģʽ������ѭ��*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*���ȼ�����*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  

		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*����DMA1��4ͨ��*/		   
		DMA_Init(DMA1_Channel5, &DMA_InitStructure); 	   
		
		/*ʹ��DMA*/
		DMA_Cmd (DMA1_Channel5,ENABLE);					
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE_2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(macUSARTx_2, &USART_InitStructure); 
	
	/*	�����ж����ȼ� */
	NVIC_Configuration();
	/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(macUSARTx_2, USART_IT_RXNE, DISABLE);
	USART_ITConfig(macUSARTx_2, USART_IT_TC, DISABLE);
USART_ITConfig(macUSARTx_2, USART_IT_IDLE, ENABLE);
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(macUSARTx_2, ENABLE);
	//USART_ClearFlag(macUSARTx_2, USART_FLAG_TC);
}

