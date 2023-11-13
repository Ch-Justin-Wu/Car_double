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
inline void chassis::XYZ_speed_set()
{
    //Vx setting
    if (Xbox.R_Trigger == 0 && Xbox.L_Trigger==0)
    {
        vx_set = 0;
    }

    else if ((Xbox.R_Trigger) > 0 && Xbox.L_Trigger == 0)
    {
        vx_set=(Xbox.R_Trigger)*K_VX_SET;
    }
    //-x�����ٶ�����
    else if ((Xbox.L_Trigger) > 0 && Xbox.R_Trigger == 0)
    {
        vx_set = -(Xbox.L_Trigger) * K_VX_SET;
    }
    //Vy setting

    //Wz setting
    L_Joystick_Difference(&Left_Joystick);
    if (Left_Joystick.H_diff==0)
    {
        wz_set = 0;
    }
    
    else if (Left_Joystick.H_diff)
    {
        wz_set = -(Left_Joystick.H_diff);
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
inline void chassis::Mec_chassis_wheel_speed(fp32 vx_set,fp32 vy_set,fp32 wz_set)
{
    wheel_speed[0] = -vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[1] = vx_set - vy_set + (CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[2] = vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
    wheel_speed[3] = -vx_set + vy_set + (-CHASSIS_WZ_SET_SCALE - 1.0f) * MOTOR_DISTANCE_TO_CENTER * wz_set;
}

// MAX wheel_speed=300 RPM*60 mm*PI
