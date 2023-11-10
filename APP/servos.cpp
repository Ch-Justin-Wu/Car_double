#include "servos.h"
using namespace std;

Servos Servo[4];

/**
 * ************************************************************************
 * @brief ������ʼ��
 *
 * @param[in] __Servos_TIM  �����ʱ��
 * @param[in] __Servos_Channel  PWMͨ��
 *
 * ************************************************************************
 */
void Servos::Init(TIM_HandleTypeDef __Servos_TIM, uint32_t __Servos_Channel, float __angle)
{
    angle = __angle;
    pwmVal = 500 + 2000.0f * __angle / 180.0f;
    Servos_TIM = __Servos_TIM;
    Servos_Channel = __Servos_Channel;
    HAL_TIM_PWM_Start(&__Servos_TIM, __Servos_Channel);
}

/**
 * ************************************************************************
 * @brief ���������PWM
 *
 * @param[in] _angle_  �趨�ĽǶ�
 *
 * ************************************************************************
 */
void Servos::PWM_Val_Cacl(float _angle_)
{
    angle = _angle_;
    pwmVal = 500 + 2000.0f * _angle_ / 180.0f;
}

/**
 * ************************************************************************
 * @brief �������
 *
 *
 * ************************************************************************
 */
void Servos::Control_Servo(float _angle_)
{
    angle = _angle_;
    pwmVal = 500 + 2000.0f * _angle_ / 180.0f;
    __HAL_TIM_SET_COMPARE(&Servos_TIM, Servos_Channel, pwmVal);
}

// #define K_Claw 0.001
#define Claw_MAX 100
#define Claw_MIN 60
void Servos::Control_Claw()
{
    if (Xbox.RB && angle <= Claw_MAX)
    {
        angle += 1;
        if (angle > Claw_MAX)
        {
            angle = Claw_MAX;
        }
    }

    if (Xbox.LB && angle >= Claw_MIN)
    {
        angle -= 1;
        if (angle < Claw_MIN)
        {
            angle = Claw_MIN;
        }
    }
    Control_Servo(angle);
}
