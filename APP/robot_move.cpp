/**
 * ************************************************************************
 *
 * @file robot_move.cpp
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief ִ�л����˿�������
 * @version 1.0
 * @date 2023-11-30
 *
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu
 * For study and research only, no reprinting
 * ************************************************************************
 */
#include "robot_move.h"

// #define OPEN
#define PID_ENABLED
#define XBOX_ENABLED

namespace RobotControl
{
	volatile uint8_t k_claw = 0;
	volatile uint8_t k_gimbal = 0;
	volatile uint8_t k_arm = 0;
	volatile uint8_t k_wrist = 0;
	volatile uint8_t g_servo_delay_1 = 0;
	//volatile uiznt8_t g_servo_delay_2 = 0;
	/**
	 * ************************************************************************
	 * @brief һ�����ƻ�е��
	 *
	 *
	 * ************************************************************************
	 */
	void one_key_control_robotic_arm()
	{
		// һ��׼��ץȡ/��λ
		if (Xbox.Share)
		{
			Servo[0].control_servo(120);
			Servo[1].control_servo(44);
			Servo[2].control_servo(74);
		}
		// һ����̨��λ
		if (Xbox.X)
		{
			Servo[1].control_servo(44);
			Servo[0].control_servo(120);
		}
		// һ����̨�Ұ�
		if (Xbox.B)
		{
			Servo[1].control_servo(44);
			Servo[0].control_servo(33);
		}
		// һ��������
		if (Xbox.Menu)
		{
			Servo[1].control_servo(44);
			Servo[2].control_servo(13);
			Servo[0].control_servo(30);
		}
		//һ��׼��Ͷ��
		if (Xbox.Xbox)
		{
			Servo[1].control_servo(44);
			Servo[2].control_servo(110);
		}
		
		// һ��׼��������
		if (Xbox.View)
		{
			const uint8_t delay_period = 60;
			g_servo_delay_1 += 1;


			Servo[2].control_servo(135);

			if (g_servo_delay_1 == delay_period)
			{
				Servo[1].control_servo(73);
				g_servo_delay_1 = 0;
			}
		}
	}
	/**
	 * ************************************************************************
	 * @brief ���ƻ�е��
	 *
	 *
	 * ************************************************************************
	 */
	void control_robotic_arm()
	{
		const uint8_t claw_angle_period = 1;
		const uint8_t gimbal_angle_period = 4;
		const uint8_t arm_angle_period = 2;
		const uint8_t wrist_angle_period = 1;
		// ���ƻ�еצ
		k_claw++;
		if (k_claw == claw_angle_period)
		{
			Servo[3].control_claw();
			k_claw = 0;
		}
		// ������̨
		k_gimbal++;
		if (k_gimbal == gimbal_angle_period)
		{
			Servo[0].control_gimbal();
			k_gimbal = 0;
		}
		// ���Ʊ�
		k_arm++;
		if (k_arm == arm_angle_period)
		{
			Servo[1].control_arm();
			k_arm = 0;
		}
		// ������
		k_wrist++;
		if (k_wrist == wrist_angle_period)
		{
			Servo[2].control_wrist();
			k_wrist = 0;
		}
	}
} /* namespace RobotControl */

/**
 * @brief Timer interrupt callback function
 *
 * @param htim Timer handle structure pointer
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{

		
#if defined(XBOX_ENABLED)
		Xbox.controller_data_rx();
		RobotControl::Mec_Chassis.controller_speed_set();
		RobotControl::control_robotic_arm();
		RobotControl::one_key_control_robotic_arm();
#endif
#if defined(ROS)
		ROS2_data_rx();
		RobotControl::Mec_Chassis.ROS2_Speed_Set(&Ros2);
#endif

		// J-Scope
		// for (uint8_t i = 0; i < 4; i++)
		// {
		// 	Test_moto[i].get = motors[i].get_rpm;
		// 	Test_moto[i].set = motors[i].set_rpm;
		// 	Test_moto[i].before_filter_get = motors[i].ori_rpm;
		// 	motors[i].motor_pwm_tx(i);
		// }

#if defined(PID_ENABLED)

		RobotControl::Mec_Chassis.mec_chassis_wheel_speed();
		for (uint8_t i = 0; i < 4; i++)
		{
			motors[i].motor_pwm_tx(i);
		}
#endif

#if defined(OPEN)
		{

			RobotControl::Mec_Chassis.mec_chassis_wheel_speed();
			for (uint8_t i = 0; i < 4; i++)
			{
				motors[i].wheel_speed_to_pwm(i);
			}
		}
#endif
	}
}
