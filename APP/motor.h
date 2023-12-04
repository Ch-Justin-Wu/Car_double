/**
 * ************************************************************************
 *
 * @file motor.h
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief ����ඨ��
 * @version 1.0
 * @date 2023-11-30
 *
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu
 * For study and research only, no reprinting
 * ************************************************************************
 */
#pragma once

#include "chassis_move.h"
#include "encoder.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_usart.h"
#include "stm32f1xx_hal.h"
#include "mytype.h"
#include "pid.h"
#include "kalman.h"

#define motor_num 4

// PID param
#define MAX_OUTPUT_LIMIT 1800
#define INTEGRAL_LIMIT 1600
#define DEADBAND 2
#define RPM_DEADBAND 80

#define MAX_ERROR 0

#define KP_MOTOR1 12.0f
#define KI_MOTOR1 3.0f
#define KD_MOTOR1 7.0f

#define KP_MOTOR2 12.0f
#define KI_MOTOR2 3.0f
#define KD_MOTOR2 5.0f

#define KP_MOTOR3 12.0f
#define KI_MOTOR3 3.0f
#define KD_MOTOR3 5.0f

#define KP_MOTOR4 12.0f
#define KI_MOTOR4 3.0f
#define KD_MOTOR4 7.0f

#define SEPA_INTEGRAL 100

	typedef __attribute((aligned(2))) struct
	{
		int16_t set;
		int16_t get;
		int16_t before_filter_get;
	} pid_test;

	extern pid_test Test_moto[motor_num];
#ifdef __cplusplus
}

#define PI 3.143592f
// ���Ĭ�Ϸ���
#define NEGATIVE 0
#define POSITIVE 1

enum speed_direction
{
	pos = 1,
	stop = 0,
	neg = -1
};

#define MAX_RPM 350
class motor
{
private:
	int16_t pwmVal;
	// ���������ʱ�����
	TIM_HandleTypeDef Driver_PWM1_TIM;
	TIM_HandleTypeDef Driver_PWM2_TIM;
	// ��ʱ��ͨ��
	uint8_t Driver_PWM1_TIM_Channel_x;
	uint8_t Driver_PWM2_TIM_Channel_x;
	// �������������ű��
	GPIO_TypeDef *Encoder_GPIOx;
	uint16_t Encoder_GPIO_Pin;

	void wheel_linear_speed_to_rpm(uint8_t i);
	float calculate_ori_rpm();
	int16_t calculate_tempVAL(uint8_t i);
	void set_direction();
	void set_pwm(uint16_t in1, uint16_t in2);

public:
	int16_t get_rpm;
	int16_t set_rpm;
	int16_t ori_rpm;
	int8_t Set_speed_direction;
	int16_t Hall_Encoder_Count;
	uint8_t Speed_Default_Direction;

	void Init(TIM_HandleTypeDef __Driver_PWM1_TIM, uint8_t __Driver_PWM1_TIM_Channel_x,
			  TIM_HandleTypeDef __Driver_PWM2_TIM, uint8_t __Driver_PWM2_TIM_Channel_x,
			  GPIO_TypeDef *__Encoder_GPIOx, uint16_t __Encoder_GPIO_Pin,
			  uint8_t __Speed_Default_Direction);
	void motor_pwm_tx(uint8_t i);
	// ����������
	void Encoder_Count();
	// �����ٶ�ת��ΪPWM
	void wheel_speed_to_pwm(uint8_t i);
};

extern motor motors[motor_num];

#endif
