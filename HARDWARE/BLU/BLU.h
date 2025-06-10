#ifndef __BLU_H
#define __BLU_H

#include "stdio.h"	
#include "sys.h" 
#include "string.h" 

#define BLU_DATA_MAX 128//���շ�����󳤶�

enum REV_Sta{//����״̬
	REV_WAIT,//�ȴ�����
	REV_ING,//���ڽ���
	REV_OK,//�������
	REV_ERR,//���մ���
};

typedef struct BLU_Typedef_Data{//���ݿ���
	uint8_t blu_flage;//״̬
	uint16_t blu_len;//���ݳ���
	uint8_t blu_data[BLU_DATA_MAX];//����
}BLU_Typedef_Data;

void blu_init(void);//������ʼ��
uint8_t Get_BLU_Sta(void);//��ȡ״̬
uint8_t Get_BLU_Len(void);//��ȡ�������ݳ���
uint8_t Get_BLU_Data(uint8_t* data);//��ȡ����(��ȡ���������)
void Send_Data(uint8_t* data,uint16_t len);//��������

#endif

