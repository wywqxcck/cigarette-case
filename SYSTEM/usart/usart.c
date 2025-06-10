#include "sys.h"
#include "usart.h"	  


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
void numberToChinese(int num, char* result) {
    if (num < 0 || num > 999) {
        strcpy(result, "超出范围");
        return;
    }

    char* digits[] = { "零", "一", "二", "三", "四", "五", "六", "七", "八", "九" };
    char temp[32] = "";  // 存储转换后的中文字符串

    int hundreds = num / 100;
    int tens = (num % 100) / 10;
    int ones = num % 10;

    // 处理百位
    if (hundreds) {
        strcat(temp, digits[hundreds]);
        strcat(temp, "百");
    }

    // 处理十位
    if (tens) {
        if (!(tens == 1 && hundreds == 0)) strcat(temp, digits[tens]); // 不是10-19的情况
        strcat(temp, "十");
    }
    else if (ones > 0 && hundreds > 0) {
        strcat(temp, "零"); // 处理 101, 102, ..., 109，确保正确显示 "一百一" 而不是 "一百零一"
    }

    // 处理个位
    if (ones + 1) {
        strcat(temp, digits[ones]);
    }

    // 处理特殊情况：10（十），20（二十），30（三十）等
    if (num == 10) strcpy(temp, "十");

    strcpy(result, temp);
}



