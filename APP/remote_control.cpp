#include "remote_control.h"
using namespace std;

#define ABS(x) ((x > 0) ? (x) : (-x))

ControllerJoystick_t Left_Joystick, Right_Joystick = {0};
remote_control Xbox;

/**
 * ************************************************************************
 * @brief ������ҡ�˽ṹ���ʼ��
 *
 * @param[in] Joystick  ������ҡ�˽ṹ��ָ��
 * @param[in] __deadband  ҡ������
 * @param[in] __H_stick_offset_position  ҡ��ˮƽ�����ʼλ��
 * @param[in] __V_stick_offset_position  ҡ�˴�ֱ�����ʼλ��
 *
 * ************************************************************************
 */
void Init_Controller_Joystick(ControllerJoystick_t *Joystick,
							  int __deadband, int __H_stick_offset_position, int __V_stick_offset_position)
{
	Joystick->deadband = __deadband;
	Joystick->H_stick_offset_position = __H_stick_offset_position;
	Joystick->V_stick_offset_position = __V_stick_offset_position;
}

/**
 * ************************************************************************
 * @brief ��ҡ�˱仯���ȼ���
 *
 * @param[in] Joystick  ������ҡ�˽ṹ��ָ��
 *
 * ************************************************************************
 */
void L_Joystick_Difference(ControllerJoystick_t *Joystick)
{
	// ˮƽ����
	Joystick->H_diff = Xbox.L_Joystick_Hor - Joystick->H_stick_offset_position;
	// ��������
	if (Joystick->H_diff != 0 && ABS(Joystick->H_diff) < Joystick->deadband)
		Joystick->H_diff = 0;
	else if (Joystick->H_diff >= Joystick->deadband)
	{
		Joystick->H_diff = Joystick->H_diff - Joystick->deadband;
	}
	else if (Joystick->H_diff <= -(Joystick->deadband))
	{
		Joystick->H_diff = Joystick->H_diff + Joystick->deadband;
	}

	// ��ֱ����
	Joystick->V_diff = Xbox.L_Joystick_Hor - Joystick->V_stick_offset_position;
	// ��������
	if (Joystick->V_diff != 0 && ABS(Joystick->V_diff) < Joystick->deadband)
		Joystick->V_diff = 0;
	else if (Joystick->V_diff >= Joystick->deadband)
	{
		Joystick->V_diff = Joystick->V_diff - Joystick->deadband;
	}
	else if (Joystick->V_diff <= -(Joystick->deadband))
	{
		Joystick->V_diff = Joystick->V_diff + Joystick->deadband;
	}
}

/**
 * ************************************************************************
 * @brief ��ҡ�˱仯���ȼ���
 *
 * @param[in] Joystick  ������ҡ�˽ṹ��ָ��
 *
 * ************************************************************************
 */
void R_Joystick_Difference(ControllerJoystick_t *Joystick)
{
	// ˮƽ����
	Joystick->H_diff = Xbox.R_Joystick_Hor - Joystick->H_stick_offset_position;
	// ��������
	if (Joystick->H_diff != 0 && ABS(Joystick->H_diff) < Joystick->deadband)
		Joystick->H_diff = 0;
	else if (Joystick->H_diff >= Joystick->deadband)
	{
		Joystick->H_diff = Joystick->H_diff - Joystick->deadband;
	}
	else if (Joystick->H_diff <= -(Joystick->deadband))
	{
		Joystick->H_diff = Joystick->H_diff + Joystick->deadband;
	}

	// ��ֱ����
	Joystick->V_diff = Xbox.R_Joystick_Ver - Joystick->V_stick_offset_position;
	// ��������
	if (Joystick->V_diff != 0 && ABS(Joystick->V_diff) < Joystick->deadband)
		Joystick->V_diff = 0;
	else if (Joystick->V_diff >= Joystick->deadband)
	{
		Joystick->V_diff = Joystick->V_diff - Joystick->deadband;
	}
	else if (Joystick->V_diff <= -(Joystick->deadband))
	{
		Joystick->V_diff = Joystick->V_diff + Joystick->deadband;
	}
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
enum ButtonState
getButtonState(uint8_t data, uint8_t mask)
{
	return (data & mask) ? BUTTON_PRESSED : BUTTON_NOT_PRESSED;
}

/**
 * ************************************************************************
 * @brief ����ҡ�˳�ʼֵ
 * 
 * 
 * ************************************************************************
 */
void remote_control::Init()
{
	L_Joystick_Hor = OFFSET_POSITION;
	L_Joystick_Ver = OFFSET_POSITION;
	R_Joystick_Hor = OFFSET_POSITION;
	R_Joystick_Ver = OFFSET_POSITION;
}

/**
 * ************************************************************************
 * @brief Receive data processing �������ݴ���
 *
 * 
 * ************************************************************************
 */
void remote_control::controller_data_resolve(void)
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
		L_Joystick_Hor = rx_p[1] | (rx_p[2] << 8);
		// The vertical travel of the left stick
		L_Joystick_Ver = rx_p[3] | (rx_p[4] << 8);
		// Horizontal stroke of the right stick
		R_Joystick_Hor = rx_p[5] | (rx_p[6] << 8);
		// The vertical stroke of the right stick
		R_Joystick_Ver = rx_p[7] | (rx_p[8] << 8);
		// Left trigger travel
		L_Trigger = rx_p[9] | (rx_p[10] << 8);
		// Right trigger stroke
		R_Trigger = rx_p[11] | (rx_p[12] << 8);
		// Joystick input combination
		combination = rx_p[13];
		// Buttons
		// ����
		A = getButtonState(rx_p[14], 0x01);		  // ���ð�ť A ��״̬
		B = getButtonState(rx_p[14], 0x02);		  // ���ð�ť B ��״̬
		X = getButtonState(rx_p[14], 0x08);		  // ���ð�ť X ��״̬
		Y = getButtonState(rx_p[14], 0x10);		  // ���ð�ť Y ��״̬
		LB = getButtonState(rx_p[14], 0x40);	  // ���ð�ť LB ��״̬
		RB = getButtonState(rx_p[14], 0x80);	  // ���ð�ť RB ��״̬
		View = getButtonState(rx_p[15], 0x04);	  // ���ð�ť View ��״̬
		Menu = getButtonState(rx_p[15], 0x08);	  // ���ð�ť Menu ��״̬
		Xbox = getButtonState(rx_p[15], 0x10);	  // ���ð�ť Xbox ��״̬
		press_L = getButtonState(rx_p[15], 0x20); // ���ð�ť press_L ��״̬
		press_R = getButtonState(rx_p[15], 0x40); // ���ð�ť press_R ��״̬
		Share = getButtonState(rx_p[16], 0x01);	  // ���ð�ť Share ��״̬
	}
	else
	{
		err = 1;
	}
}

/**
 * ************************************************************************
 * @brief �����ֱ����ݰ�
 * 
 * 
 * ************************************************************************
 */
void remote_control::controller_data_rx(void)
{
	if (recv_end_flag == 1 && rx_len == DATA_FRAME_LENGTH)
	{
		controller_data_resolve();

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

// B-red X-blue
// void remote_control::Set_color(void)
// {
	// if (B == 1 && X == 0)
	// {
	// 	Red_flag = 1;
	// 	Blue_flag = 0;
	// }
	// else if (B == 0 && X == 1)
	// {
	// 	Red_flag = 0;
	// 	Blue_flag = 1;
	// }

	// if (Red_flag == 1 && Blue_flag == 0)
	// {
	// 	ws2812_red(LED_NUM);
	// }
	// else if (Red_flag == 0 && Blue_flag == 1)
	// {
	// 	ws2812_blue(LED_NUM);
	// }
// }
