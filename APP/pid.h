/**
 * ************************************************************************
 * 
 * @file pid.h
 * @author Justin Wu (justin.wu@zllcrm.org)
 * @brief pid�ṹ�嶨��
 * @version 1.0
 * @date 2023-11-30
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2023 Justin Wu 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f1xx_hal.h"
  // ö��
  enum
  {
    LAST = 0,
    NOW = 1,
  };

  enum Flag
  {
    OFF = 0,
    ON = 1
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
    float max_output;

    /* pid����������޷� */
    float integral_limit;
    /* pid���ַ������� */
    float separationThreshold;
    // ����ӦKp
    float k1;
    float k2;
    float k3;
  } pid_t;

  void pid_init(pid_t *pid, uint32_t max_out, uint32_t intergral_limit, float Deadband, float Max_err, float kp, float ki, float kd, uint16_t __separationThreshold);

  // void pid_reset(pid_t *pid, float kp, float ki, float kd);

  float pid_calc(pid_t *pid, float get, float set);

  extern pid_t pid_motor[];

#ifdef __cplusplus
}
#endif
