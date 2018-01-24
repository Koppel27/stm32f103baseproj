#ifndef __LCD_H__
#define __LCD_H__

#include "bsp_common.h"
#include "Common.h"
#include "board.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

#define   HIGH           320
#define   LENGTH         480

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


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

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);									//��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//����

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����

extern void CslLCD_WR_REG(uint16_t Reg);
extern void CslLCD_WR_DATA(uint16_t Data);
extern void CslLCD_MRD_DATA(uint16_t* Reg, int Count);
extern void CslLCD_MWR_DATA(uint16_t* Data, int Count);

extern void CslLCD_BLK(unsigned char NewStatus);

#endif
