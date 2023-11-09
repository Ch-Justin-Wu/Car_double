#include "remote_control.h"

Controller_t xbox_t = {0};
ControllerJoystick_t Left_Joystick, Right_Joystick = {0};

void Init_Controller_Joystick(ControllerJoystick_t *Joystick,
							  int __deadband, int __stick_offset_position)
{
	Joystick->deadband = __deadband;
	Joystick->stick_offset_position = __stick_offset_position;
}

/**
 * @brief Get the state of a button from the given data using a bitmask.
 *
 * This function checks whether a specific bit (button) in the data is set (pressed)
 * or clear (not pressed) based on the provided mask.
 *
 * @param[in] data The data containing button states.
 * @param[in] mask The bitmask representing the button to be checked.
 * @return The state of the button (BUTTON_PRESSED or BUTTON_NOT_PRESSED).
 */
// ���尴ť״̬��ö��
enum ButtonState getButtonState(uint8_t data, uint8_t mask)
{
	return (data & mask) ? BUTTON_PRESSED : BUTTON_NOT_PRESSED;
}

/**
 * ************************************************************************
 * @brief Receive data processing �������ݴ���
 *
 * @param[in] ptr  �������ṹ��ָ��
 *
 * ************************************************************************
 */
static void Data_Resolve(Controller_t *ptr)
{

	// ����
	uint8_t temp_cnt = 0;

	// Takes the cached array address
	uint8_t *rx_p = rx_buffer;

	// Find header 0xA5 with loop optimization
	while (*rx_p != 0xA5 && temp_cnt <= 19)
	{
		temp_cnt++;
		rx_p++;
	}

	// Error flag
	err = 0;
	// Determine the frame header and end of the frame
	// �ж�֡ͷ��֡β
	if (rx_p[0] == 0xA5 && rx_p[17] == 0xA6)
	{

		// Horizontal stroke of the left stick
		ptr->L_Joystick_Hor = rx_p[1] | (rx_p[2] << 8);
		// The vertical travel of the left stick
		ptr->L_Joystick_Ver = rx_p[3] | (rx_p[4] << 8);
		// Horizontal stroke of the right stick
		ptr->R_Joystick_Hor = rx_p[5] | (rx_p[6] << 8);
		// The vertical stroke of the right stick
		ptr->R_Joystick_Ver = rx_p[7] | (rx_p[8] << 8);
		// Left trigger travel
		ptr->L_Trigger = rx_p[9] | (rx_p[10] << 8);
		// Right trigger stroke
		ptr->R_Trigger = rx_p[11] | (rx_p[12] << 8);
		// Joystick input combination
		ptr->combination = rx_p[13];
		// Buttons
		// ����
		ptr->A = getButtonState(rx_p[14], 0x01);	   // ���ð�ť A ��״̬
		ptr->B = getButtonState(rx_p[14], 0x02);	   // ���ð�ť B ��״̬
		ptr->X = getButtonState(rx_p[14], 0x08);	   // ���ð�ť X ��״̬
		ptr->Y = getButtonState(rx_p[14], 0x10);	   // ���ð�ť Y ��״̬
		ptr->LB = getButtonState(rx_p[14], 0x40);	   // ���ð�ť LB ��״̬
		ptr->LB = getButtonState(rx_p[14], 0x80);	   // ���ð�ť RB ��״̬
		ptr->View = getButtonState(rx_p[15], 0x04);	   // ���ð�ť View ��״̬
		ptr->Menu = getButtonState(rx_p[15], 0x08);	   // ���ð�ť Menu ��״̬
		ptr->Xbox = getButtonState(rx_p[15], 0x10);	   // ���ð�ť Xbox ��״̬
		ptr->press_L = getButtonState(rx_p[15], 0x20); // ���ð�ť press_L ��״̬
		ptr->press_R = getButtonState(rx_p[15], 0x40); // ���ð�ť press_R ��״̬
		ptr->Share = getButtonState(rx_p[16], 0x01);   // ���ð�ť Share ��״̬
	}
	else
	{
		err = 1;
	}
}

void controller_data(void)
{
	if (recv_end_flag == 1 && rx_len == DATA_FRAME_LENGTH)
	{
		Data_Resolve(&xbox_t);

		if (err == 1)
		{
			// printf("The data length is wrong!\n");
		}

		// Clear the count
		//  �������
		rx_len = 0;
		// Clear the Receive End flag
		//  ������ս�����־
		recv_end_flag = 0;

		// Clear the receive cache to set each byte in the receive buffer to 0
		// ������ջ���,�����ջ�������ÿ���ֽڶ���0
		memset(rx_buffer, 0, rx_len);
	}
	// Turn DMA reception back on
	//  ���´�DMA����
	HAL_UART_Receive_DMA(&c_huart, rx_buffer, BUF_SIZE);
}
