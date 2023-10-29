/**
 * ************************************************************************
 *
 * @file bsp_usart.c
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief ���ݰ����մ���ʹ�ӡ
 * @version 1.0
 * @date 2023-10-16
 *
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu
 * For study and research only, no reprinting
 * ************************************************************************
 */
#include "bsp_usart.h"
#include "stm32f1xx_it.h"
#include "stdio.h"



// Error flag
// �����־λ
uint8_t err;
// The length of one frame of data received
// ����һ֡���ݵĳ���
volatile uint8_t rx_len = 0;
// A frame of data reception completion flag
// һ֡���ݽ�����ɱ�־
volatile uint8_t recv_end_flag = 0;

// Define the serial port receiving buffer
//  ���崮�ڽ��ջ�����
uint8_t rx_buffer[BUF_SIZE] = {0};


/**
 * ************************************************************************
 * @brief Serial port receive function ���ڽ��շ�װ
 *
 * @param[in] Data ���ݵ�ַ Data address
 * @param[in] len  ���ݳ��� Data length
 *
 * ************************************************************************
 */
void DMA_Usart_Rx(uint8_t *Data, uint8_t len)
{
	// Turn DMA reception back on
	//  ���´�DMA����
	HAL_UART_Receive_DMA(&c_huart, Data, len);
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART2_IRQHandler(void)
{

	uint32_t tmp_flag = 0;
	uint32_t temp;

	// Get the IDLE flag bit
	tmp_flag = __HAL_UART_GET_FLAG(&c_huart, UART_FLAG_IDLE);

	if (tmp_flag != RESET)
	{
		// Clear the IDLE flag in UART
		__HAL_UART_CLEAR_IDLEFLAG(&c_huart);

		// Clear the status register (SR)
		temp = c_huart.Instance->SR;

		// Read data from DR (Data Register)
		temp = c_huart.Instance->DR;

		HAL_UART_DMAStop(&c_huart); // Stop DMA transfer

		// Get the number of untransmitted data in DMA
		temp = c_dma.Instance->CNDTR;

		// Calculate the number of received data by subtracting the total count from the untransmitted data count
		rx_len = BUF_SIZE - temp;

		// Set the receive completion flag to 1
		recv_end_flag = 1;
	}

	HAL_UART_IRQHandler(&c_huart);
}

int fputc(int ch, FILE *f)

{

	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);

	return ch;
}

/**
 * ************************************************************************
 * @brief @brief Print Controller Data ��ӡ����������
 *
 *
 * ************************************************************************
 */
// void Print_Controller_Data(Controller_t *ptr)
// {

// 	// ��ӡҡ�˺Ͱ������
// 	printf("ҡ��ˮƽ�г�:\n");
// 	printf("��: %d\n", ptr->L_Joystick[0]);
// 	printf("��: %d\n", ptr->R_Joystick[0]);
// 	printf("ҡ�˴�ֱ�г�:\n");
// 	printf("��: %d\n", ptr->L_Joystick[1]);
// 	printf("��: %d\n", ptr->R_Joystick[1]);

// 	// ��ӡ����г�
// 	printf("����г�:\n");
// 	printf("��: %d\n", ptr->L_Trigger);
// 	printf("��: %d\n", ptr->R_Trigger);

// 	// ��ӡ��ť״̬
// 	printf("�����������:\n");
// 	for (int i = 0; i < sizeof(buttonNames) / sizeof(buttonNames[0]); i++)
// 	{
// 		if (ptr->buttons[i] == BUTTON_PRESSED)
// 		{
// 			printf("���� %s\n", buttonNames[i]);
// 		}
// 	}

// 	printf("Gamepad arrow keys input combination:\n");
// 	switch (ptr->combination)
// 	{
// 	case 0:
// 		printf("������\n");
// 		break;
// 	case 1:
// 		printf("��\n");
// 		break;
// 	case 2:
// 		printf("��+��\n");
// 		break;
// 	case 8:
// 		printf("��+��\n");
// 		break;
// 	case 5:
// 		printf("��\n");
// 		break;
// 	case 4:
// 		printf("��+��\n");
// 		break;
// 	case 6:
// 		printf("��+��\n");
// 		break;
// 	case 3:
// 		printf("��\n");
// 		break;
// 	case 7:
// 		printf("��\n");
// 		break;
// 	default:
// 		printf("Data error!\n");
// 		break;
// 	}
// }

