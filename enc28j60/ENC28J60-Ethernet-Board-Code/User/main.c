#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "SPI.H"
#include "simple_server.h"
#include "usart.h"


/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2

#define BP2             0x2000                     // PC13: BP2
#define BP3             0x0001                     // PA0 : BP3

#define UP              0x0800                     // PB11: UP
#define RIGHT           0x1000                     // PB12: RIGHT
#define LEFT            0x2000                     // PB13: LEFT
#define DOWN            0x4000                     // PB14: DOWN
#define OK              0x8000                     // PB15: OK

#define JOYSTICK        0xF800                     // JOYSTICK ALL KEYS
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/
/*----------------------------------------------------------*\
 |  Delay                                                   |
 |  延时 Inserts a delay time.                              |
 |  nCount: 延时时间                                        |
 |  nCount: specifies the delay time length.                |
\*----------------------------------------------------------*/
void Delay(vu32 nCount)
{
   for (; nCount != 0; nCount--);
}
/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar(int ch)
{
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (ch & 0x1FF);

   return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey(void)
{
   while (!(USART1->SR & USART_FLAG_RXNE));

   return ((int) (USART1->DR & 0x1FF));
}

int main(void)
{
	USART_Configuration();
	 SPInet_Init();
	 simple_server();
	 return 0;
}
