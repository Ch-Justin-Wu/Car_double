#include "robot_move.h"
using namespace std;

// #define OPEN
 #define PID
		#define xbox

uint16_t K_Claw = 0;
uint16_t K_Gimbal = 0;
uint16_t K_Arm = 0;
uint16_t K_Wrist = 0;

void Control_Robotic_Arm();

/**
 * @brief Timer interrupt callback function
 *
 * @param htim Timer handle structure pointer
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{

		K_Claw++;
		K_Gimbal++;
		K_Arm++;
		K_Wrist++;
#if defined(xbox)
		Xbox.controller_data_rx();
		Mec_Chassis.controller_speed_set();
#endif
#if defined(ROS)
		ROS2_data_rx();
		Mec_Chassis.ROS2_Speed_Set(&Ros2);
#endif

		// J-Scope
		// for (uint8_t i = 0; i < 4; i++)
		// {
		// 	Test_moto[i].get = motors[i].get_rpm;
		// 	Test_moto[i].set = motors[i].set_rpm;
		// 	Test_moto[i].before_filter_get = motors[i].ori_rpm;
		// 	motors[i].motor_pwm_tx(i);
		// }

#if defined(PID)

		Mec_Chassis.mec_chassis_wheel_speed();
		for (uint8_t i = 0; i < 4; i++)
		{

			motors[i].wheel_linear_speed_to_rpm(i);
			motors[i].motor_pwm_tx(i);
		}
#endif

#if defined(OPEN)
		{

			Mec_Chassis.mec_chassis_wheel_speed();
			for (uint8_t i = 0; i < 4; i++)
			{
				motors[i].wheel_speed_to_pwm(i);
			}
		}
#endif
		// ��е�ۿ���
		Control_Robotic_Arm();
	}
}

/**
 * ************************************************************************
 * @brief ���ƻ�е��
 *
 *
 * ************************************************************************
 */
inline void Control_Robotic_Arm()
{
	// һ��׼��ץȡ/��λ
	if (Xbox.Share)
	{
		Servo[0].Control_Servo(120);
		Servo[1].Control_Servo(44);
		Servo[2].Control_Servo(74);
	}
	// һ����̨��λ
	if (Xbox.X)
	{
		Servo[1].Control_Servo(44);
		Servo[0].Control_Servo(120);
	}
	// һ����̨�Ұ�
	if (Xbox.B)
	{
		Servo[1].Control_Servo(44);
		Servo[0].Control_Servo(33);
	}
	// һ��������
	if (Xbox.Menu)
	{
		Servo[1].Control_Servo(44);
		Servo[2].Control_Servo(13);
		Servo[0].Control_Servo(30);
	}

	// ���ƻ�еצ
	if (K_Claw == 1)
	{
		Servo[3].Control_Claw();
		K_Claw = 0;
	}
	// ������̨
	if (K_Gimbal == 4)
	{
		Servo[0].Control_Gimbal();
		K_Gimbal = 0;
	}
	// ���Ʊ�
	if (K_Arm == 2)
	{
		Servo[1].Control_Arm();
		K_Arm = 0;
	}
	// ������
	if (K_Wrist == 1)
	{
		Servo[2].Control_Wrist();
		K_Wrist = 0;
	}
}
