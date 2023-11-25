#ifndef kalman_H
#define kalman_H

#include "main.h"
#include "motor.h"
typedef struct
{
    float Last_P; // �ϴι���Э���� ������Ϊ0 ! ! ! ! !
    float Now_P;  // ��ǰ����Э����
    float out;    // �������˲������
    float Kg;     // ����������
    float Q;      // ��������Э����
    float R;      // �۲�����Э����
} kalman;

void kalman_init(kalman *ptr);
 float kalman_filter(kalman *kfp, float input);

extern kalman kfp[];

#endif
