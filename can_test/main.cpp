//功能：接收从CAN1发送过来的数据，并打印到终端



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "controlcan.h"

#include <ctime>
#include <cstdlib>
#include "unistd.h"


main()
{

	printf(">>this is mytestdemo !\r\n");//指示程序已运行

	if(VCI_OpenDevice(VCI_USBCAN2,0,0)==1)//打开设备
	{
		printf(">>open deivce success!\n");//打开设备成功
	}else
	{
		printf(">>open deivce error!\n");
		exit(1);
	}

	//配置CAN
	VCI_INIT_CONFIG config; 
	config.AccCode = 0x80000000;
	config.AccMask = 0xFFFFFFFF;
	config.Filter = 2;   //只接收标准帧
	config.Timing0 = 0x01; 
	config.Timing1 = 0x1C; //波特率定为250kps  0x01 0x1C
	config.Mode = 0; //正常模式		

	//初始化CAN1:VCI_InitCAN(设备类型、设备索引、CAN通道索引、CAN配置参数)
	if(VCI_InitCAN(VCI_USBCAN2,0,0,&config)!=1)
	{
		printf(">>Init CAN1 error\n");
		VCI_CloseDevice(VCI_USBCAN2,0);
	}

	if(VCI_StartCAN(VCI_USBCAN2,0,0)!=1)
	{
		printf(">>Start CAN1 error\n");
		VCI_CloseDevice(VCI_USBCAN2,0);

	}

	//初始化用来接收的数据帧，帧结构体数组的长度设置为50
    VCI_CAN_OBJ rec[50];
	int reclen=0;
	int i,j;


	while(1)
	{

		//VCI_InitCAN(设备类型、设备索引、CAN通道索引0就是CAN1、接收数组的首指针rec、接收数组的长度50、保留参数)	
		if((reclen = VCI_Receive(VCI_USBCAN2,0,0,rec,50,200)) >= 0)//调用接收函数，如果有数据，则进行处理
		{
			for(i=0; i<reclen; i++)
			{
				//打印数据
				printf("data:0x");	
				for( j= 0; j < rec[i].DataLen; j++ )
				{
					printf(" %02X", rec[i].Data[j]); //%02X表示用2个位置数据一个16进制数
				}
				printf("\n");
			}
		}
	}
}

