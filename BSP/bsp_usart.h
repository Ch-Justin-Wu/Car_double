#ifndef __BSP_USART_H__
#define __BSP_USART_H__
#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "dma.h"

// Defines the serial port receive buff length
//  ���崮�ڽ���buff����,��ֹDMA����Խ��
#define BUF_SIZE 36u
#define DATA_FRAME_LENGTH 18u

// Use DEFINE to improve code portability
// ��define��ߴ�������ֲ��
#define c_huart huart2
#define c_UART USART2
#define c_dma hdma_usart2_rx

    void My_USART2_Init(void);

    extern DMA_HandleTypeDef c_dma;
    extern uint8_t *rx_p;
    extern uint8_t err;
    extern volatile uint8_t rx_len;
    extern volatile uint8_t recv_end_flag;
    extern uint8_t rx_buffer[BUF_SIZE];
#ifdef __cplusplus
}

#endif

#endif
