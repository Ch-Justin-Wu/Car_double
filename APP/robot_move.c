#include "robot_move.h"

/**
 * @brief Timer interrupt callback function
 *
 * @param htim Timer handle structure pointer
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		control_robot();
	
	

	// ���pid���Ʋ���
	// pid_reset(&pid_angle[1], kp_ang_2, ki_ang_2, kd_ang_2);
	// ���ٶȼ���

	// angle_setspeed_1=pid_calc(&pid_angle[0], (float)moto_chassis[0].total_angle/ 8192.0f * 360.0f-(float)moto_chassis[1].total_angle/ 8192.0f * 360.0f, 0.0f);
	//  PID �������ؼ���

	short torque1 = pid_calc(&pid_motor[0], (float)moto_chassis[0].speed_rpm, 0);
	// can_cmd_send(torque1, torque2);
	// ���ٶȼ���
	// angle_setspeed_2 = angle_speed_cacl(&moto_chassis[1],1, set_round2);
	// �������ǶȲ���� PID��� ʵ�ֵ���������
	// angle_setspeed_2=pid_calc(&pid_angle[1], (float)moto_chassis[1].total_angle/ 8192.0f * 360.0f-(float)moto_chassis[0].total_angle/ 8192.0f * 360.0f, 0.0f);

	//  PID �������ؼ���

	short torque2 = pid_calc(&pid_motor[1], (float)moto_chassis[1].speed_rpm, 0);
	// real_speed2 = moto_chassis[1].speed_rpm;
}
}


