#include "lcd.h"
#include "font.h"

//LCD_FSMC_D0   D14
//LCD_FSMC_D1   D15
//LCD_FSMC_D2   D0
//LCD_FSMC_D3   D1
//LCD_FSMC_D4   E7
//LCD_FSMC_D5   E8
//LCD_FSMC_D6   E9
//LCD_FSMC_D7   E10
//LCD_FSMC_D8   E11
//LCD_FSMC_D9   E12
//LCD_FSMC_D10  E13
//LCD_FSMC_D11  E14
//LCD_FSMC_D12  E15
//LCD_FSMC_D13  D8
//LCD_FSMC_D14  D9
//LCD_FSMC_D15  D10
//LCD_RD        D4
//LCD_WR        D5
//LCD_RS        D11
//LCD_CS        D7

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

static void Delay(unsigned int time)                                                                       
{
	volatile unsigned int i;
	
	while(time--)
	{
		for(i=0;i<1000;i++);
	}
}

void LCD_lowlevel_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
#if BOARD_TYPE == RELEASE_BOARD_V1
	RCC_APB2PeriphClockCmd(GPIO2RCC(_LCD_BLK_GPIO), ENABLE);
#endif
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  |
                             GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
	                           GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                             GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                           GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 2;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 1;
	
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_BLK(unsigned char NewStatus)
{
#if BOARD_TYPE == RELEASE_BOARD_V1
	if(NewStatus)
	{
		GPIO_SetBits(_LCD_BLK_GPIO, _LCD_BLK_PIN);
	}
	else
	{
		GPIO_ResetBits(_LCD_BLK_GPIO, _LCD_BLK_PIN);
	}
#endif
}

void LCD_Init(void)
{
	LCD_lowlevel_Init();
	CSL_LCD_WriteComm(0x01);
	Delay(1200);
	CSL_LCD_WriteComm(0xE0);
	CSL_LCD_WriteData(0x00);
	CSL_LCD_WriteData(0x07);
	CSL_LCD_WriteData(0x0f);
	CSL_LCD_WriteData(0x0D);
	CSL_LCD_WriteData(0x1B);
	CSL_LCD_WriteData(0x0A);
	CSL_LCD_WriteData(0x3c);
	CSL_LCD_WriteData(0x78);
	CSL_LCD_WriteData(0x4A);
	CSL_LCD_WriteData(0x07);
	CSL_LCD_WriteData(0x0E);
	CSL_LCD_WriteData(0x09);
	CSL_LCD_WriteData(0x1B);
	CSL_LCD_WriteData(0x1e);
	CSL_LCD_WriteData(0x0f); 
	
	CSL_LCD_WriteComm(0xE1);
	CSL_LCD_WriteData(0x00);
	CSL_LCD_WriteData(0x22);
	CSL_LCD_WriteData(0x24);
	CSL_LCD_WriteData(0x06);
	CSL_LCD_WriteData(0x12);
	CSL_LCD_WriteData(0x07);
	CSL_LCD_WriteData(0x36);
	CSL_LCD_WriteData(0x47);
	CSL_LCD_WriteData(0x47);
	CSL_LCD_WriteData(0x06);
	CSL_LCD_WriteData(0x0a);
	CSL_LCD_WriteData(0x07);
	CSL_LCD_WriteData(0x30);
	CSL_LCD_WriteData(0x37);
	CSL_LCD_WriteData(0x0f);
	
	CSL_LCD_WriteComm(0xC0);
	CSL_LCD_WriteData(0x10);
	CSL_LCD_WriteData(0x10);
	
	CSL_LCD_WriteComm(0xC1);
	CSL_LCD_WriteData(0x41);
	
	CSL_LCD_WriteComm(0xC5);
	CSL_LCD_WriteData(0x00);
	CSL_LCD_WriteData(0x22);
	CSL_LCD_WriteData(0x80);
	
	CSL_LCD_WriteComm(0x36);
	//CSL_LCD_WriteData(0x68);
	//CSL_LCD_WriteData(0x48);
	CSL_LCD_WriteData(0x28);//20171218 yangzw
	
	CSL_LCD_WriteComm(0x3A);//Interface Mode Control
	CSL_LCD_WriteData(0x55);
	
	CSL_LCD_WriteComm(0xB0);//Interface Mode Control  
	CSL_LCD_WriteData(0x00);
	CSL_LCD_WriteComm(0xB1);//Frame rate 70HZ  
	CSL_LCD_WriteData(0xB0);
	CSL_LCD_WriteData(0x11);
	CSL_LCD_WriteComm(0xB4);
	CSL_LCD_WriteData(0x02);  
	CSL_LCD_WriteComm(0xB6);//RGB/MCU Interface Control
	CSL_LCD_WriteData(0x02);
	CSL_LCD_WriteData(0x02);
	
	CSL_LCD_WriteComm(0xB7);
	CSL_LCD_WriteData(0xC6);
	
	//WriteComm(0xBE);
	//WriteData(0x00);
	//WriteData(0x04);
	
	CSL_LCD_WriteComm(0xE9); 
	CSL_LCD_WriteData(0x00);

	CSL_LCD_WriteComm(0xF7);
	CSL_LCD_WriteData(0xA9);
	CSL_LCD_WriteData(0x51);
	CSL_LCD_WriteData(0x2C); 
	CSL_LCD_WriteData(0x82);

	CSL_LCD_WriteComm(0x11);
	Delay(120);
	CSL_LCD_WriteComm(0x29);
	
	LCD_BLK(1);
}

/* 设置光标位置 */
void BlockWrite(unsigned int Xstart, unsigned int Xend, unsigned int Ystart, unsigned int Yend)
{
	CSL_LCD_WriteComm(0x2a);
	CSL_LCD_WriteData(Xstart>>8);
	CSL_LCD_WriteData(Xstart&0xff);
	CSL_LCD_WriteData(Xend>>8);
	CSL_LCD_WriteData(Xend&0xff);

	CSL_LCD_WriteComm(0x2b);
	CSL_LCD_WriteData(Ystart>>8);
	CSL_LCD_WriteData(Ystart&0xff);
	CSL_LCD_WriteData(Yend>>8);
	CSL_LCD_WriteData(Yend&0xff);

	CSL_LCD_WriteComm(0x2c);
}

/* draw point func */
void SetPixel(unsigned int x,unsigned int y)
{
	BlockWrite(x,x+1,y,y+1);
	CSL_LCD_WriteData(POINT_COLOR);
}
void Fast_SetPixel(unsigned int x,unsigned int y, unsigned int color)
{
	BlockWrite(x,x+1,y,y+1);
	CSL_LCD_WriteData(color & 0xFFFF);
}

/* read point func */
unsigned int GetPixel(unsigned int x, unsigned int y)
{
	return 0;
}

void CslLCD_WR_REG(uint16_t Reg)
{
	CSL_LCD_WriteComm(Reg);
}

void CslLCD_WR_DATA(uint16_t Data)
{
	CSL_LCD_WriteData(Data);
}

void CslLCD_MRD_DATA(uint16_t* pData, int Count)
{
	volatile uint16_t dummy;
	uint16_t read1,read2,read3;
	uint16_t r1,g1,b1,r2,g2,b2;
	
	dummy = CSL_LCD_RAM;
	while(Count>1)
	{
		read1 = CSL_LCD_RAM;
		read2 = CSL_LCD_RAM;
		read3 = CSL_LCD_RAM;
		r1 = read1 >> 11;
		g1 = read1 >> 2 & 0x3F;
		b1 = read2 >> 11;
		r2 = read2 >> 3 & 0x1F;
		g2 = read3 >> 10;
		b2 = read3 >> 3 & 0x1F;
		*pData++ = (r1<<11) + (g1<<5) + b1;
		*pData++ = (r2<<11) + (g2<<5) + b2;
		Count-=2;
	}
	if(Count)
	{
		read1 = CSL_LCD_RAM;
		read2 = CSL_LCD_RAM;
		r1 = read1 >> 11;
		g1 = read1 >> 2 & 0x3F;
		b1 = read2 >> 11;
		*pData++ = (r1<<11) + (g1<<5) + b1;
	}
}

void LCD_MWR_DATA(uint16_t* pData, int Count)
{
	while(Count--)
		CSL_LCD_WriteData(*pData++);
}

/* 清屏 */
void LCD_Clear(uint16_t BackColor)
{
	uint32_t i;
	
	BlockWrite(0, 480, 0, 320);
	for(i=0;i<480*320;i++)
	{
		CSL_LCD_WriteData(BackColor);
	}
}
/* 画矩形填充 */
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
 
void LCD_FillRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t Color)
{
	uint32_t i;
	
	BlockWrite(xStart, xEnd, yStart, yEnd);
	for(i=0;i<(xEnd-xStart+1)*(yEnd-yStart+1);i++)
	{
		CSL_LCD_WriteData(Color);
	}
}

/************************** add by yang 20171123 ********************************/ 
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
//		LCD_DrawPoint(uRow,uCol);//画点
		SetPixel(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		SetPixel(x0+a,y0-b);             //5
 		SetPixel(x0+b,y0-a);             //0           
		SetPixel(x0+b,y0+a);             //4               
		SetPixel(x0+a,y0+b);             //6 
		SetPixel(x0-a,y0+b);             //1       
 		SetPixel(x0-b,y0+a);             
		SetPixel(x0-a,y0-b);             //2             
  		SetPixel(x0-b,y0-a);             //7 
		
//		LCD_DrawPoint(x0+a,y0-b);             //5
// 		LCD_DrawPoint(x0+b,y0-a);             //0           
//		LCD_DrawPoint(x0+b,y0+a);             //4               
//		LCD_DrawPoint(x0+a,y0+b);             //6 
//		LCD_DrawPoint(x0-a,y0+b);             //1       
// 		LCD_DrawPoint(x0-b,y0+a);             
//		LCD_DrawPoint(x0-a,y0-b);             //2             
//  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)Fast_SetPixel(x,y,POINT_COLOR);
			else if(mode==0)Fast_SetPixel(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y >= HIGH )return;		//超区域了   
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x >= LENGTH )return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


