#ifndef __PID_H
#define __PID_H

#include "stm32f1xx_hal.h"
//ö��
enum
{
  LAST = 0,
  NOW = 1,
};

/**
 * @brief     PID �ṹ��
 */
typedef struct
{
  /* p��i��d���� */
  float p;
  float i;
  float d;

  /* Ŀ��ֵ������ֵ�����ֵ */
  float set;
  float get;
  float err[2];

  /* p��i��d������������� */
  float pout;
  float iout;
  float dout;

  /* pid��ʽ������������ */
  float out;

  float max_err;
  float deadband; // err < deadband return
  /* pid����������  */
  uint32_t max_output;

  /* pid����������޷� */
  uint32_t integral_limit;

} pid_t;

/**
 * @brief     PID ��ʼ������
 * @param[in] pid: PID �ṹ��
 * @param[in] max_out: ������
 * @param[in] intergral_limit: �����޷�
 * @param[in] kp/ki/kd: ���� PID ����
 */
void pid_init(pid_t *pid, uint32_t max_out, uint32_t intergral_limit, float Deadband, float Max_err, float kp, float ki, float kd);

/**
 * @brief     PID ������λ����
 * @param[in] pid: PID �ṹ��
 * @param[in] kp/ki/kd: ���� PID ����
 */
void pid_reset(pid_t *pid, float kp, float ki, float kd);

/**
 * @brief     PID ���㺯����ʹ��λ��ʽ PID ����
 * @param[in] pid: PID �ṹ��
 * @param[in] get: ��������
 * @param[in] set: Ŀ������
 * @retval    PID �������
 */
float pid_calc(pid_t *pid, float get, float set);

extern pid_t pid_motor[];
extern pid_t pid_angle[];
// extern pid_t pid_motor2;
//extern pid_t pid_angle2;
#endif
