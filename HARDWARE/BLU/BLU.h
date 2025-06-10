#ifndef __BLU_H
#define __BLU_H

#include "stdio.h"	
#include "sys.h" 
#include "string.h" 

#define BLU_DATA_MAX 128//接收发送最大长度

enum REV_Sta{//接收状态
	REV_WAIT,//等待接收
	REV_ING,//正在接收
	REV_OK,//接收完成
	REV_ERR,//接收错误
};

typedef struct BLU_Typedef_Data{//数据控制
	uint8_t blu_flage;//状态
	uint16_t blu_len;//数据长度
	uint8_t blu_data[BLU_DATA_MAX];//数据
}BLU_Typedef_Data;

void blu_init(void);//蓝牙初始化
uint8_t Get_BLU_Sta(void);//获取状态
uint8_t Get_BLU_Len(void);//获取接收数据长度
uint8_t Get_BLU_Data(uint8_t* data);//获取数据(获取完数据清空)
void Send_Data(uint8_t* data,uint16_t len);//发送数据

#endif

