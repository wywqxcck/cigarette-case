#include "BLU.h"

static BLU_Typedef_Data BLU_Data;


void blu_init(void){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOAʱ��
  
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.3

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���2
	
	memset(BLU_Data.blu_data, 0, BLU_DATA_MAX);//�������
	BLU_Data.blu_flage = REV_WAIT;//����Ϊ�ȴ�����״̬
	BLU_Data.blu_len = 0;//���ճ���Ϊ0
}


void Send_Data(uint8_t* data,uint16_t len)
{
	uint8_t temp[BLU_DATA_MAX];
	temp[0] = 0xA5;
	uint8_t verify = 0;
	for(uint16_t i = 1;i <= len;i++){
		temp[i] = data[i-1];
		verify += data[i-1];
	}
	temp[len+1] = verify;
	temp[len+2] = 0x5A;
	
	for(uint16_t i = 0;i < len + 3;i++){
		USART_SendData(USART3, temp[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}

uint8_t Get_BLU_Sta(void)
{
	return BLU_Data.blu_flage;
}

uint8_t Get_BLU_Len(void)
{
	return BLU_Data.blu_len;
}

uint8_t Get_BLU_Data(uint8_t* data)
{
	if(Get_BLU_Sta() != REV_OK){
		return REV_ERR;
	}
	for(uint16_t i = 0;i < BLU_Data.blu_len;i++){
		data[i] = BLU_Data.blu_data[i];
	}
	memset(BLU_Data.blu_data, 0, BLU_DATA_MAX);
	BLU_Data.blu_flage = REV_WAIT;
	BLU_Data.blu_len = 0;
	return REV_OK;
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res,verify = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������9
		switch(BLU_Data.blu_flage){
			case REV_WAIT:
				if(Res == 0xA5){
					BLU_Data.blu_flage = REV_ING;
				}
				break;
			case REV_ING:
				if(Res != 0x5A){
					BLU_Data.blu_data[BLU_Data.blu_len++] = Res;
				}
				else{
					verify = 0;
					for(uint16_t i = 0;i < BLU_Data.blu_len - 1;i++){
						verify += BLU_Data.blu_data[i];
					}
					if(verify == BLU_Data.blu_data[BLU_Data.blu_len - 1]){
						BLU_Data.blu_len--;
						BLU_Data.blu_data[BLU_Data.blu_len - 1] = '\0';
						BLU_Data.blu_flage = REV_OK;
					}
					else{
						BLU_Data.blu_flage = REV_ERR;
					}
				}
				if(BLU_Data.blu_len >= BLU_DATA_MAX){
					BLU_Data.blu_flage = REV_ERR;
				}
				break;
			case REV_ERR:
				memset(BLU_Data.blu_data, 0, BLU_DATA_MAX);
				BLU_Data.blu_flage = REV_WAIT;
				BLU_Data.blu_len = 0;
				break;
		}
	}
} 

