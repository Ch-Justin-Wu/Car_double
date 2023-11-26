#include "pid.h"
#include <math.h>
#include "motor.h"

#define ABS(x) ((x > 0) ? (x) : (-x))
// ����ٶ� PID �ṹ�嶨��
pid_t pid_motor[motor_num] = {0}; // �ٶȻ�
// pid_t pid_angle[motor_num] = {0}; //�ǶȻ�

void abs_limit(float *a, float ABS_MAX)
{
    if (*a > ABS_MAX)
        *a = ABS_MAX;
    if (*a < -ABS_MAX)
        *a = -ABS_MAX;
}

/**
 * @brief ��ʼ��PID����������
 *
 * @param pid       Ҫ��ʼ����PID������
 * @param max_out   ����������
 * @param integral_limit ��������
 * @param Deadband  ����ֵ
 * @param Max_err   ������
 * @param kp        ����ϵ��
 * @param ki        ����ϵ��
 * @param kd        ΢��ϵ��
 * @param __separationThreshold ���ַ���ϵ��
 */
void pid_init(pid_t *pid, uint32_t max_out, uint32_t intergral_limit, float Deadband, float Max_err, float kp, float ki, float kd, uint16_t __separationThreshold)
{
    pid->integral_limit = intergral_limit;
    pid->max_output = max_out;
    pid->deadband = Deadband;
    pid->max_err = Max_err;

    pid->p = kp;
    pid->i = ki;
    pid->d = kd;

    pid->separationThreshold = __separationThreshold;

    pid->k1 = 5;
    pid->k2 = 53;
    pid->k3 = 10;
}

/**
 * @brief     PID ���㺯����ʹ��λ��ʽ PID ����
 * @param[in] pid: PID �ṹ��
 * @param[in] get: ��������
 * @param[in] set: Ŀ������
 * @retval    PID �������
 */
float pid_calc(pid_t *pid, float get, float set)
{
    // Set the current error value
    pid->get = get;
    pid->set = set;
    pid->err[NOW] = set - get;

    // Check if the current error is beyond the maximum error limit
    if (pid->max_err != 0 && ABS(pid->err[NOW]) > pid->max_err)
        return 0;

    // Check if the current error is within the deadband range
    if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
        return 0; // Deadband control

    // ����ӦKp���� auto control Kp
    if (pid->k1 != 0 || pid->k2 != 0 || pid->k3 != 0)
    {
        pid->p = pid->k1 * log10f(pid->k2 * ABS(pid->err[NOW] + pid->k3));
    }

    // Calculate the proportional component
    pid->pout = pid->p * pid->err[NOW];

    if (pid->separationThreshold != 0 && ABS(pid->err[NOW]) > pid->separationThreshold || ABS(pid->set) < 150) // �����ȶ�
    {
        pid->iout = 0;
    }

    else if (!pid->separationThreshold || ABS(pid->err[NOW]) <= pid->separationThreshold)
    {
        // Update the integral component
        pid->iout += pid->i * pid->err[NOW];
    }

    // Calculate the derivative component
    pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST]);

    // Limit the integral component
    abs_limit(&(pid->iout), pid->integral_limit);

    // Calculate the total control output
    pid->out = pid->pout + pid->iout + pid->dout;

    // Limit the total control output
    abs_limit(&(pid->out), pid->max_output);

    // Update the error for the next iteration
    pid->err[LAST] = pid->err[NOW];

    return pid->out;
}

/**
 * @brief     PID ������λ����
 * @param[in] pid: PID �ṹ��
 * @param[in] kp/ki/kd: ���� PID ����
 */
// void pid_reset(pid_t *pid, float kp, float ki, float kd)
// {
//     pid->p = kp;
//     pid->i = ki;
//     pid->d = kd;

//     // pid->pout = 0;
//     // pid->iout = 0;
//     // pid->dout = 0;
//     // pid->out = 0;
// }
