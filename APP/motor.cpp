/**
 * ************************************************************************
 *
 * @file motor.cpp
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief �������
 * @version 1.0
 * @date 2023-11-30
 *
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu
 * For study and research only, no reprinting
 * ************************************************************************
 */
#include "motor.h"

motor motors[motor_num];
pid_test Test_moto[motor_num] = {0};

/**
 * @brief �����ֵ
 *
 * @tparam type
 * @param x ��������
 * @return type x�ľ���ֵ
 */
template <typename type>
type math_abs(type x)
{
	return ((x > 0) ? x : -x);
}

/**
 * ************************************************************************
 * @brief ��ʼ�������
 *
 * @param[in] __Driver_PWM1_TIM  PWM1��ʱ��
 * @param[in] __Driver_PWM1_TIM_Channel_x  PWM1��ͨ��
 * @param[in] __Driver_PWM2_TIM  PWM2��ʱ��
 * @param[in] __Driver_PWM2_TIM_Channel_x  PWM2��ͨ��
 * @param[in] __Encoder_GPIOx  ����������������
 * @param[in] __Encoder_GPIO_Pin  �������������ź�
 * @param[in] __Speed_Direction_GPIOx  ����ٶȷ���������
 * @param[in] __Speed_Direction_GPIO_Pin  ����ٶȷ������ź�
 * @param[in] __Speed_Default_Direction ���Ĭ�Ϸ���
 *
 * ************************************************************************
 */
void motor::Init(TIM_HandleTypeDef __Driver_PWM1_TIM, uint8_t __Driver_PWM1_TIM_Channel_x,
				 TIM_HandleTypeDef __Driver_PWM2_TIM, uint8_t __Driver_PWM2_TIM_Channel_x,
				 GPIO_TypeDef *__Encoder_GPIOx, uint16_t __Encoder_GPIO_Pin,

				 uint8_t __Speed_Default_Direction)
{
	Driver_PWM1_TIM = __Driver_PWM1_TIM;
	Driver_PWM1_TIM_Channel_x = __Driver_PWM1_TIM_Channel_x;
	Driver_PWM2_TIM = __Driver_PWM2_TIM;
	Driver_PWM2_TIM_Channel_x = __Driver_PWM2_TIM_Channel_x;
	Encoder_GPIOx = __Encoder_GPIOx;
	Encoder_GPIO_Pin = __Encoder_GPIO_Pin;
	Speed_Default_Direction = __Speed_Default_Direction;

	HAL_TIM_PWM_Start(&__Driver_PWM1_TIM, __Driver_PWM1_TIM_Channel_x);
	HAL_TIM_PWM_Start(&__Driver_PWM2_TIM, __Driver_PWM2_TIM_Channel_x);
}

/**
 * ************************************************************************
 * @brief ����ԭʼ���ת��
 *
 *
 * @return ԭʼ���ת��
 * ************************************************************************
 */
float motor::calculate_ori_rpm()
{
	return Hall_Encoder_Count / 13.0 / 2.0 / 30.0 * 100 * 60;
}

/**
 * ************************************************************************
 * @brief ����pid���ı�pwmռ�ձ�
 *
 * @param[in] i  ������
 *
 * @return pid���ֵ
 * ************************************************************************
 */
int16_t motor::calculate_tempVAL(uint8_t i)
{
	return pid_calc(&pid_motor[i], (float)get_rpm, (float)set_rpm);
}

/**
 * ************************************************************************
 * @brief ���õ����������
 *
 *
 * ************************************************************************
 */
void motor::set_direction()
{
	// ��˥��
	if (set_rpm - get_rpm > RPM_DEADBAND)
	{

		Set_speed_direction = 1;
	}

	else if (set_rpm - get_rpm < -RPM_DEADBAND)
	{

		Set_speed_direction = -1;
	}

	else if (math_abs(set_rpm - get_rpm) <= RPM_DEADBAND)
	{
		if (set_rpm > 0)
		{
			Set_speed_direction = 1;
		}

		else if (set_rpm < 0)
		{
			Set_speed_direction = -1;
		}

		// else if (set_rpm==0||math_abs(get_rpm)<=1)
		// {
		// 	Set_speed_direction = 0;
		// }
	}
}

void motor::set_pwm(uint16_t in1, uint16_t in2)
{
	__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, in1);
	__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, in2);
}

/**
 * ************************************************************************
 * @brief ���õ��ת�٣�pid�������PWM
 *	IN1		IN2		����
 *	0		0		���У�����
 *	1		0		����
 *	0		1		����
 *	1		1		ɲ��
 *
 * PWM		0		��תPWM����˥��
 * 1		PWM		��תPWM����˥��
 * 0		PWM		��תPWM����˥��
 * PWM		1		��תPWM����˥��
 *
 * @param[in] i  ������
 * ************************************************************************
 */
void motor::motor_pwm_tx(uint8_t i)
{

	int16_t tempVAL = 0;
	const int16_t const_VAL = 1800;

	wheel_linear_speed_to_rpm(i);
	// Real_rpm
	ori_rpm = calculate_ori_rpm();
	Hall_Encoder_Count = 0;
	// Kalman filter
	get_rpm = kalman_filter(&kfp[i], ori_rpm);

	tempVAL = calculate_tempVAL(i);

	set_direction();

	switch (Set_speed_direction)
	{
		// ��ת
	case -1:
		pwmVal = -tempVAL + const_VAL;
		set_pwm(0, pwmVal);
		break;
		// ��ת
	case 1:
		pwmVal = tempVAL + const_VAL;
		set_pwm(pwmVal, 0);
		break;

	case 0:
		// �������
		set_pwm(0, 0);
		// ���ɲ��
		// set_pwm(2 * const_VAL, 2 * const_VAL);
		break;

	default:
		break;
	}
}

/**
 * ************************************************************************
 * @brief ����������������
 *
 *
 * ************************************************************************
 */
void motor::Encoder_Count()
{

	if (Set_speed_direction > 0)
	{
		Hall_Encoder_Count++;
	}
	else if (Set_speed_direction < 0)
	{
		Hall_Encoder_Count--;
	}
}

/**
 * ************************************************************************
 * @brief
 *
 * @param[in] i  ������
 *
 * ************************************************************************
 */
void motor::wheel_linear_speed_to_rpm(uint8_t i)
{
	const fp32 range = 25000.0f;
	set_rpm = RobotControl::Mec_Chassis.wheel_speed[i] / range * MAX_RPM;
	if (set_rpm >= MAX_RPM)
	{
		set_rpm = MAX_RPM;
	}
	else if (set_rpm <= -MAX_RPM)
	{
		set_rpm = -MAX_RPM;
	}
}

/**
 * ************************************************************************
 * @brief ������������pwm
 *
 * 	IN1		IN2		����
 *	0		0		���У�����
 *	1		0		����
 *	0		1		����
 *	1		1		ɲ��
 *
 * PWM		0		��תPWM����˥��
 * 1		PWM		��תPWM����˥��
 * 0		PWM		��תPWM����˥��
 * PWM		1		��תPWM����˥��
 *
 *  @param[in] i  ������
 * ************************************************************************
 */
void motor::wheel_speed_to_pwm(uint8_t i)
{
	const fp32 range = 25000.0f;
	const fp32 max_pwm_val = 3600;
	pwmVal = math_abs(RobotControl::Mec_Chassis.wheel_speed[i]) / range * max_pwm_val;
	if (pwmVal >= max_pwm_val)
	{
		pwmVal = max_pwm_val;
	}

	if (RobotControl::Mec_Chassis.wheel_speed[i] > 0)
	{
		switch (Speed_Default_Direction)
		{
		case POSITIVE:
			__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, pwmVal);
			__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, 0);
			break;
		case NEGATIVE:
			__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, pwmVal);
			__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, 0);
			break;
		default:
			break;
		}
	}
	else if (RobotControl::Mec_Chassis.wheel_speed[i] < 0)
	{
		switch (Speed_Default_Direction)
		{
		case POSITIVE:
			__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, 0);
			__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, pwmVal);
			break;
		case NEGATIVE:
			__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, 0);
			__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, pwmVal);
			break;
		default:
			break;
		}
	}
	else if (RobotControl::Mec_Chassis.wheel_speed[i] == 0)
	{
		// �������
		__HAL_TIM_SET_COMPARE(&Driver_PWM1_TIM, Driver_PWM1_TIM_Channel_x, 0);
		__HAL_TIM_SET_COMPARE(&Driver_PWM2_TIM, Driver_PWM2_TIM_Channel_x, 0);
	}
}
