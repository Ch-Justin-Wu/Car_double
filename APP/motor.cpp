
#include "motor.h"

using namespace std;

motor motors[motor_num] = {0};

// ��ʼ�����
void motor::Init(TIM_HandleTypeDef __Driver_PWM1_TIM, uint8_t __Driver_PWM1_TIM_Channel_x,
				 TIM_HandleTypeDef __Driver_PWM2_TIM, uint8_t __Driver_PWM2_TIM_Channel_x,
				 GPIO_TypeDef *__Encoder_GPIOx, uint16_t __Encoder_GPIO_Pin,
				 GPIO_TypeDef *__Speed_Direction_GPIOx, uint16_t __Speed_Direction_GPIO_Pin)
{
	Driver_PWM1_TIM = __Driver_PWM1_TIM;
	Driver_PWM1_TIM_Channel_x = __Driver_PWM1_TIM_Channel_x;
	Driver_PWM2_TIM = __Driver_PWM2_TIM;
	Driver_PWM2_TIM_Channel_x = __Driver_PWM2_TIM_Channel_x;
	encoder.Encoder_GPIOx = __Encoder_GPIOx;
	encoder.Encoder_GPIO_Pin = __Encoder_GPIO_Pin;
	encoder.Speed_Direction_GPIOx = __Speed_Direction_GPIOx;
	encoder.Speed_Direction_GPIO_Pin = __Speed_Direction_GPIO_Pin;
	HAL_TIM_PWM_Start(&__Driver_PWM1_TIM, __Driver_PWM1_TIM_Channel_x);
	HAL_TIM_PWM_Start(&__Driver_PWM2_TIM, __Driver_PWM2_TIM_Channel_x);
}

// ����Ϊʵ��ת��
void motor::Get_rpm()
{
	for (uint8_t i = 0; i < motor_num; i++)
	{
		motors[i].rpm = motors[i].encoder.Hall_Encoder_Count / 13.0 / 2.0 * 100 * 60;
		motors[i].encoder.Hall_Encoder_Count = 0;
	}
}
// ���õ��ת��
void motor::Set_rpm()
{

	short pwmVal[motor_num] = {0};

	for (uint8_t i = 0; i < motor_num; i++)
	{
		pwmVal[i] = pid_calc(&pid_motor[i], (float)motors[i].rpm, (float)motors[i].set_rpm);
	}
}

// MotorData_t motors[motor_num];

// /**
//  * @brief ��ʼ���������
//  */
// void init_motor_data() {
//     for (int i = 0; i < motor_num; i++) {
//         motors[i].real_total_angle = 0.0f;
//         motors[i].angle_setspeed = 0.0f;
//         motors[i].set_angle = 0.0f;
//         motors[i].actual_round = 0.0f;
//         motors[i].pid_angle = &pid_angle[i];
//     }
// }

// /**
//  * @brief ���ٶȼ���
//  *
//  * @param moto_chassis
//  * @param k
//  * @param set_round
//  * @return float
//  */
// float angle_speed_cacl(moto_measure_t moto_chassis[], u8 k, float set_round)
// {
//     MotorData_t* motor = &motors[k];

//     get_total_angle(&moto_chassis[k]);
//     // ���ٱ�3591/187  �����趨�ܽǶ�
//     motor->set_angle = (float)set_round * 360.0f * 3591.0f / 187.0f;

//     // ����ʵ���ܽǶ�
//     motor->real_total_angle = (float)moto_chassis[k].total_angle / 8192.0f * 360.0f;

//     motor->actual_round = (float)moto_chassis[k].total_angle / 8192.0f * 187.0f / 3591.0f;

//     // ͨ���ǶȻ������趨(��)�ٶ�
//     motor->angle_setspeed = pid_calc(motor->pid_angle, motor->real_total_angle, motor->set_angle);

//     return motor->angle_setspeed;
// }

// float real_total_angle[motor_num];
// float angle_setspeed[motor_num];
// float set_angle[motor_num];
// // float actual_round;

// /**
//  * @brief ���ٶȼ���
//  *
//  * @param set_round
//  * @return float
//  */
// float angle_speed_cacl(moto_measure_t moto_chassis[], u8 k, float set_round)
// {

//     set_angle[k] = set_round * 360.0f * 3591.0f / 187.0f;                  // ���ٱ�3591/187  �����趨�ܽǶ� 36.0f/1.0f
//     real_total_angle[k] = moto_chassis[k].total_angle / 8192.0f * 360.0f; // ����ʵ���ܽǶ�
//     actual_round[k] = (float)moto_chassis[k].total_angle / 8192.0f * 187.0f / 3591.0f;
//     angle_setspeed[k] = pid_calc(&pid_angle[k], real_total_angle[k], set_angle[k]); // ͨ���ǶȻ������趨(��)�ٶ�
//     return angle_setspeed[k];
// }
