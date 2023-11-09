#include "chassis_move.h"

using namespace std;
chassis Mec_Chassis;

/**
 * ************************************************************************
 * @brief 
 * 
 * @param[in] ptr  �ֱ��ṹ��ָ��
 * 
 * 
 * ************************************************************************
 */
void chassis::XYZ_speed_set(Controller_t* ptr)
{
    if ((ptr->R_Trigger)>3)
    {
        vx_set=(ptr->R_Trigger);
    }
    //-x�����ٶ�����
    else if ((ptr->L_Trigger)>3)
    {
        vx_set=-(ptr->L_Trigger);
    }
    
}

/**
 * ************************************************************************
 * @brief 
 * 
 * @param[in] vx_set  x�����ٶ�
 * @param[in] vy_set  y�����ٶ�
 * @param[in] wz_set  z����ٶ�
 * @param[in] wheel_speed  �������ٶ�
 * 
 * ************************************************************************
 */
void chassis::Mec_chassis_wheel_speed(fp32 vx_set,fp32 vy_set,fp32 wz_set,fp32 wheel_speed[4])
{
    wheel_speed[0] = -vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[1] = vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[2] = vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[3] = -vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
}




