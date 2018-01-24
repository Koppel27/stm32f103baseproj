#ifndef __LCD_H__
#define __LCD_H__

#include "bsp_common.h"
#include "Common.h"
#include "board.h"

//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

#define   HIGH           320
#define   LENGTH         480

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#define CSL_LCD_REG  *(volatile u16*)(0x60000000)
#define CSL_LCD_RAM  *(volatile u16*)(0x60020000)

#define CSL_LCD_WriteComm(x)  CSL_LCD_REG = x
#define CSL_LCD_WriteData(x)  CSL_LCD_RAM = x

extern void LCD_Init(void);
extern void SetPixel(unsigned int x,unsigned int y);
extern void Fast_SetPixel(unsigned int x,unsigned int y,unsigned int color);
extern void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
extern void LCD_Clear(uint16_t BackColor);
extern void LCD_FillRect(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t Color);

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);									//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体

extern void CslLCD_WR_REG(uint16_t Reg);
extern void CslLCD_WR_DATA(uint16_t Data);
extern void CslLCD_MRD_DATA(uint16_t* Reg, int Count);
extern void CslLCD_MWR_DATA(uint16_t* Data, int Count);

extern void CslLCD_BLK(unsigned char NewStatus);

#endif
