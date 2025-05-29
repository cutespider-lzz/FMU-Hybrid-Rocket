#include "stdio.h"
#include "printf.h"

UART_HandleTypeDef *USART_x = &huart1;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	
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
//重写fputc函数
int fputc(int c, FILE *stream)    
{
 /*
    huart1是工具生成代码定义的UART1结构体，
    如果以后要使用其他串口打印，只需要把这个结构体改成其他UART结构体。
*/
    HAL_UART_Transmit(USART_x, (unsigned char *)&c, 1, 1000);   
    return 1;
}
#endif

