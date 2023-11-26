#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "encoder.h"
#include "bsp_usart.h"
#include "stm32f1xx_hal.h"
#include "mytype.h"
#include "pid.h"
#include "chassis_move.h"
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
	// typedef struct
	// {
	// 	float real_total_angle;
	// 	float angle_setspeed;
	// 	float set_angle;
	// 	float actual_round;
	// 	pid_t *pid_angle;
	// } MotorData_t;

typedef struct 
{
	int16_t set;
	int16_t get;
	int16_t before_filter_get;
}pid_test;

extern pid_test Test_M1, Test_M2, Test_M3, Test_M4;
#ifdef __cplusplus
}

using namespace std;
#define PI 3.143592f
// ���Ĭ�Ϸ���

#define NEGATIVE 0
#define POSITIVE 1
#define MAX_RPM 350
class motor
{

public:
	int16_t get_rpm;
	int16_t set_rpm;

	int16_t ori_rpm;

	int16_t pwmVal;

	int8_t Set_speed_direction;
	int8_t Get_speed_direction;
	
	struct
	{
		
		int16_t Hall_Encoder_Count;

	} encoder;

	void Init(TIM_HandleTypeDef __Driver_PWM1_TIM, uint8_t __Driver_PWM1_TIM_Channel_x,
			  TIM_HandleTypeDef __Driver_PWM2_TIM, uint8_t __Driver_PWM2_TIM_Channel_x,
			  GPIO_TypeDef *__Encoder_GPIOx, uint16_t __Encoder_GPIO_Pin,
			  
			  uint8_t __Speed_Default_Direction);
	//void Real_rpm();
	void motor_pwm_tx(uint8_t i);
	void Encoder_Count();
	void wheel_linear_speed_to_rpm(uint8_t i);
	//void Speed_test(uint8_t i);
	void wheel_speed_to_pwm(uint8_t i);

	// ���������ʱ�����
	TIM_HandleTypeDef Driver_PWM1_TIM;
	TIM_HandleTypeDef Driver_PWM2_TIM;
	// ��ʱ��ͨ��
	uint8_t Driver_PWM1_TIM_Channel_x;
	uint8_t Driver_PWM2_TIM_Channel_x;
	// �������������ű��
	GPIO_TypeDef *Encoder_GPIOx;
	uint16_t Encoder_GPIO_Pin;

	uint8_t Speed_Default_Direction;
};

extern motor motors[motor_num];
#endif

#endif
