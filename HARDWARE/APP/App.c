#include "App.h"


void open_menu(void);
void Add_Menu(void);

uint16_t Data_Arr[DATA_MAX] = { 10,0,0,0 },max_num = 0xFFFF;

uint64_t old_open_time = 0,open_time = 10;

uint8_t open_flage = 0,add_flage = 1,err_flage = 0;


void App_Init(void)
{
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	blu_init();
	OLED_Init();
	DHT11_GPIO_Init();
	LOCK_Init();
	KEY_GPIO_Init();
	delay_ms(1000);
	printf("��ӭʹ�������̺�");
	TIM1_Int_Init(9,7199);
	Add_Menu();
  button_init(&btn1, read_button1_GPIO, 0);
  button_attach(&btn1, SINGLE_CLICK,BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START,BTN1_LONG_PRESS_START_Handler);
  button_start(&btn1);
}


void App_Run(void)
{
	static uint64_t read_dht11_time = 0;
	
	if(Get_BLU_Data((uint8_t*)(&max_num)) == REV_OK){
		printf("���óɹ�");
	}
	
	if(err_flage == 1){
		printf("δ������ʱ���ֹ����");
		err_flage = 0;
	}
	Add_Menu();
	if(Get_OS_Time() - read_dht11_time >= 200){
		DHT11_Read_TempAndHumidity(&DHT11Data);
		Data_Arr[T] = DHT11Data.temp_int;
		Data_Arr[H] = DHT11Data.humi_int;
		read_dht11_time = Get_OS_Time();
		Send_Data((uint8_t *)Data_Arr,DATA_MAX * 2);
	}
	char temp_text[20];
	sprintf(temp_text,"T:%.2d    H:%.2d",DHT11Data.temp_int,DHT11Data.humi_int);
	OLED_ShowString(0,0,(uint8_t *)temp_text,16);

	
	
	sprintf(temp_text,"Restrict:%.2d",Data_Arr[RESTRICT]);
	OLED_ShowString(0,2,(uint8_t *)temp_text,16);
	
	sprintf(temp_text,"In_All:%.3d",Data_Arr[IN_ALL]);
	OLED_ShowString(0,4,(uint8_t *)temp_text,16);
	
	if(open_time > ((Get_OS_Time() - old_open_time) / 1000)){
		sprintf(temp_text,"time:%.3lld",open_time - ((Get_OS_Time() - old_open_time)/1000));
	}
	else{
		sprintf(temp_text,"time:O N");
	}
	OLED_ShowString(0,6,(uint8_t *)temp_text,16);
	open_menu();
	
}


void open_menu(void)
{
	uint8_t num = 1;
	int8_t key = KEY_NO;
	char temp_text[30];
	if(open_flage){
		OLED_Clear();
		LOCK_ON();
		OLED_ShowString(32,0,(uint8_t *)"Open Menu",16);
		old_open_time = Get_OS_Time();
		while(1){
			if(Get_OS_Time() - old_open_time > 20 * 1000){
				break;
			}
			sprintf(temp_text,"num:%.2d",num);
			OLED_ShowString(42,3,(uint8_t *)temp_text,16);
			sprintf(temp_text,"%.2lld",20 - (Get_OS_Time() - old_open_time) / 1000);
			OLED_ShowString(110,6,(uint8_t *)temp_text,16);
			key = Get_Key();
			if(key != KEY_NO){
				if(key == KEY3){//ȷ��
						break;
				}
				else if(key == KEY2){
					if(num < Data_Arr[RESTRICT]){
						num++;
					}
				}
				else if(key == KEY1){
					if(num > 0){
						num--;
					}
				}
			}
			//������ȡ������
		}
		Data_Arr[RESTRICT] -= num;
		Data_Arr[IN_ALL] += num;
		OLED_Clear();
		old_open_time = Get_OS_Time();
		open_flage = 0;
		memset(temp_text,0,sizeof(temp_text) / temp_text[0]);
		numberToChinese(Data_Arr[IN_ALL],temp_text);
		printf("��������%s����ע������",temp_text);
		if(Data_Arr[IN_ALL] >= max_num){
			numberToChinese(max_num,temp_text);
			printf("�������̳���%s��",temp_text);
		}
	}
}

void Add_Menu(void)
{
	if(add_flage){
		LOCK_ON();
		OLED_Clear();
		OLED_ShowString(32,0,(uint8_t *)"Add Menu",16);
		int8_t key = KEY_NO;
		char temp_text[30];
		old_open_time = Get_OS_Time();
		while(1){
			if(Get_OS_Time() - old_open_time > 20 * 1000){
				break;
			}
			sprintf(temp_text,"%.2lld",20 - (Get_OS_Time() - old_open_time) / 1000);
			OLED_ShowString(110,6,(uint8_t *)temp_text,16);
			sprintf(temp_text,"Restrict:%.2d",Data_Arr[RESTRICT]);
			OLED_ShowString(20,3,(uint8_t *)temp_text,16);
			key = Get_Key();
			if(key != KEY_NO){
				if(key == KEY3){
					break;
				}
				else if(key == KEY2){
					if(Data_Arr[RESTRICT] < 20){
						Data_Arr[RESTRICT] += 1;
					}
				}
				else if(key == KEY1){
					if(Data_Arr[RESTRICT] > 0){
						Data_Arr[RESTRICT] -= 1;
					}
				}
			}
		}
		old_open_time = Get_OS_Time();
		add_flage = 0;
		OLED_Clear();
	}
}

/*

*/


