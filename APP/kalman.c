#include "kalman.h"

kalman kfp[motor_num]={0};

void kalman_init(kalman* ptr)
{
    ptr->Last_P = 0.001;
    ptr->Now_P = 0;
    ptr->out = 0;
    ptr->Kg = 0;
    ptr->Q = 0.0001;
    ptr->R = 0.0005;
}

/**
 *�������˲���
 *@param 	kalman *kfp �������ṹ�����
 *   			float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float kalman_filter(kalman *kfp, float input)
{
    // Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
    kfp->Now_P = kfp->Last_P + kfp->Q;
    // ���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // ��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
    kfp->out = kfp->out + kfp->Kg * (input - kfp->out); // ��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
    // ����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
    kfp->Last_P = (1 - kfp->Kg) * kfp->Now_P;
    return kfp->out;
}
